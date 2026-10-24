/* SPDX-License-Identifier: Apache-2.0 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>

#define UART_NODE DT_CHOSEN(zephyr_console)
#define LED_NODE DT_ALIAS(led0)
#define LINE_BUFFER_SIZE 32
#define RX_QUEUE_DEPTH 10
#define LED_STACK_SIZE 2048
#define LED_PRIORITY 5
#define LED_INTERVAL_MS 250

#if !DT_NODE_HAS_STATUS(UART_NODE, okay)
#error "zephyr,console must refer to an enabled UART node"
#endif
#if !DT_NODE_HAS_STATUS(LED_NODE, okay)
#error "led0 must refer to an enabled LED node"
#endif

enum line_result {
	LINE_OK,
	LINE_TOO_LONG,
	LINE_INVALID_BYTE,
	LINE_UART_ERROR,
};

struct line_message {
	enum line_result result;
	int uart_errors;
	char text[LINE_BUFFER_SIZE];
};

static const struct device *const uart_dev = DEVICE_DT_GET(UART_NODE);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

/* 完成した行（エラーも含む）をコピーして最大10件保持する。 */
K_MSGQ_DEFINE(rx_queue, sizeof(struct line_message), RX_QUEUE_DEPTH, 4);
K_THREAD_STACK_DEFINE(led_stack, LED_STACK_SIZE);
static struct k_thread led_thread;

/* UARTを有効化した後、以下の受信状態はISRだけが更新する。 */
static struct line_message rx_line;
static size_t rx_len;
static bool previous_was_cr;

/* ISR／LEDスレッドとmainの間で共有する通知だけをatomicにする。 */
static atomic_t queue_overflow;
static atomic_t led_failed;

/* この関数を呼ぶのはmainだけ。ISRとLEDスレッドから送信しない。 */
static void uart_send_string(const char *text)
{
	while (*text != '\0') {
		uart_poll_out(uart_dev, (unsigned char)*text++);
	}
}

static void record_rx_errors(const struct device *dev)
{
	int flags = uart_err_check(dev);

	/* このAPIの実装有無は、mainで割り込み有効化前に確認する。 */
	if (flags > 0) {
		rx_line.result = LINE_UART_ERROR;
		rx_line.uart_errors |= flags;
		rx_len = 0;
		previous_was_cr = false;
	}
}

static void receive_byte(uint8_t c)
{
	if (c == '\n' && previous_was_cr) {
		previous_was_cr = false;
		return;
	}
	previous_was_cr = (c == '\r');

	if (c == '\r' || c == '\n') {
		rx_line.text[rx_len] = '\0';
		/* ISRでは待たない。満杯なら今回の完成行を捨てる。 */
		if (k_msgq_put(&rx_queue, &rx_line, K_NO_WAIT) != 0) {
			atomic_set(&queue_overflow, 1);
		}
		/* キューはデータをコピーするため、このバッファを再利用できる。 */
		rx_len = 0;
		rx_line.result = LINE_OK;
		rx_line.uart_errors = 0;
		return;
	}
	if (rx_line.result != LINE_OK) {
		return;
	}
	if (c < 0x20 || c > 0x7e) {
		rx_line.result = LINE_INVALID_BYTE;
		return;
	}
	if (rx_len >= sizeof(rx_line.text) - 1) {
		rx_line.result = LINE_TOO_LONG;
		return;
	}
	rx_line.text[rx_len++] = (char)c;
}

/* MCUX UARTドライバのISRから呼ばれる。ここでは送信・sleepをしない。 */
static void serial_cb(const struct device *dev, void *user_data)
{
	uint8_t c;

	ARG_UNUSED(user_data);
	if (uart_irq_update(dev) <= 0) {
		return;
	}
	/* データなしのエラー割り込みでもフラグを確認・解除する。 */
	record_rx_errors(dev);
	if (uart_irq_rx_ready(dev) <= 0) {
		return;
	}
	while (uart_fifo_read(dev, &c, 1) == 1) {
		record_rx_errors(dev);
		receive_byte(c);
	}
}

static void led_entry(void *p1, void *p2, void *p3)
{
	int logical = 0;

	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);
	while (1) {
		int next = !logical;

		if (gpio_pin_set_dt(&led, next) < 0) {
			atomic_set(&led_failed, 1);
			return;
		}
		logical = next;
		k_msleep(LED_INTERVAL_MS);
	}
}

static void send_message(const struct line_message *message)
{
	switch (message->result) {
	case LINE_OK:
		uart_send_string("Echo: ");
		uart_send_string(message->text);
		uart_send_string("\r\n");
		break;
	case LINE_TOO_LONG:
		uart_send_string("ERROR: line too long (max 31 bytes); discarded\r\n");
		break;
	case LINE_INVALID_BYTE:
		uart_send_string("ERROR: printable ASCII only; line discarded\r\n");
		break;
	case LINE_UART_ERROR:
		if (message->uart_errors & UART_ERROR_OVERRUN) {
			uart_send_string("ERROR: UART RX overrun; line discarded\r\n");
		} else {
			uart_send_string("ERROR: UART RX error; line discarded\r\n");
		}
		break;
	}
}

int main(void)
{
	struct line_message message;
	int ret;

	if (!device_is_ready(uart_dev)) {
		return 0;
	}
	if (!gpio_is_ready_dt(&led) ||
	    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE) < 0) {
		uart_send_string("ERROR: LED setup failed; main exits.\r\n");
		return 0;
	}
	if (uart_err_check(uart_dev) < 0) {
		uart_send_string("ERROR: uart_err_check unavailable; main exits.\r\n");
		return 0;
	}
	ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);
	if (ret < 0) {
		uart_send_string("ERROR: UART IRQ callback setup failed; main exits.\r\n");
		return 0;
	}

	k_thread_create(&led_thread, led_stack, K_THREAD_STACK_SIZEOF(led_stack),
			led_entry, NULL, NULL, NULL, LED_PRIORITY, 0, K_FOREVER);
	k_thread_start(&led_thread);

	uart_send_string("Stage 6: interrupt line echo\r\n");
	uart_send_string("Local echo OFF; max 31 printable ASCII bytes; queue depth=10.\r\n");
	uart_send_string("LED toggle interval=250 ms; main waits for completed lines.\r\n");
	uart_irq_rx_enable(uart_dev);
	uart_irq_err_enable(uart_dev);

	while (k_msgq_get(&rx_queue, &message, K_FOREVER) == 0) {
		if (atomic_clear(&queue_overflow) != 0) {
			uart_send_string("ERROR: RX queue full; one or more lines discarded\r\n");
		}
		if (atomic_clear(&led_failed) != 0) {
			uart_send_string("ERROR: LED write failed; LED thread stopped\r\n");
		}
		send_message(&message);
	}
	return 0;
}
