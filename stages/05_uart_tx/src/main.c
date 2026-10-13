/* SPDX-License-Identifier: Apache-2.0 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define UART_NODE DT_CHOSEN(zephyr_console)
#define LED_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(UART_NODE, okay)
#error "zephyr,console must refer to an enabled UART node"
#endif

#if !DT_NODE_HAS_STATUS(LED_NODE, okay)
#error "led0 must refer to an enabled LED node"
#endif

#define LED_STACK_SIZE 2048
#define LED_PRIORITY 5
#define LED_INTERVAL_MS 250
#define UART_CHAR_WAIT_MS 100
#define MESSAGE_GAP_MS 2000

/* コンソールと同じUARTのデバイスを取得。準備状態は実行時に確認する。 */
static const struct device *const uart_dev = DEVICE_DT_GET(UART_NODE);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

K_THREAD_STACK_DEFINE(led_stack, LED_STACK_SIZE);
static struct k_thread led_thread;

/* 終端のNULは送らない。改行も含め、文字列の各バイトをそのまま送る。 */
static void uart_send_string(const char *text)
{
	while (*text != '\0') {
		uart_poll_out(uart_dev, (unsigned char)*text);
		/* 観察用に各文字の後で待つ。ドライバ内部のポーリングとは別。 */
		k_msleep(UART_CHAR_WAIT_MS);
		++text;
	}
}

/* mainが初期消灯を設定した後、LEDの状態変更はこのスレッドだけが行う。 */
static void led_entry(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	int logical = 0;

	while (1) {
		int next = !logical;
		int ret = gpio_pin_set_dt(&led, next);

		if (ret < 0) {
			printk("ERROR: gpio_pin_set_dt=%d; LED thread exits\n", ret);
			return;
		}
		logical = next;
		/* 待機中はCPUを譲り、GPIO出力は設定した状態を保つ。 */
		k_msleep(LED_INTERVAL_MS);
	}
}

int main(void)
{
	bool ready = device_is_ready(uart_dev);

	printk("Stage 5: UART polling transmit\n");
	printk("UART device=%s, ready=%s\n", uart_dev->name,
	       ready ? "true" : "false");
	if (!ready) {
		printk("ERROR: UART device is not ready; main exits\n");
		return 0;
	}

	bool gpio_ready = gpio_is_ready_dt(&led);

	printk("GPIO device=%s, pin=%u, ready=%s\n", led.port->name,
	       (unsigned int)led.pin, gpio_ready ? "true" : "false");
	if (!gpio_ready) {
		printk("ERROR: GPIO device is not ready; main exits\n");
		return 0;
	}
	int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	printk("gpio_pin_configure_dt=%d, initial expected=OFF\n", ret);
	if (ret < 0) {
		return 0;
	}
	printk("main priority=%d, LED priority=%d, interval_ms=%d, "
	       "LED requested_stack=%d B\n", CONFIG_MAIN_THREAD_PRIORITY,
	       LED_PRIORITY, LED_INTERVAL_MS, LED_STACK_SIZE);
	k_thread_create(&led_thread, led_stack, K_THREAD_STACK_SIZEOF(led_stack),
			led_entry, NULL, NULL, NULL, LED_PRIORITY, 0, K_FOREVER);
	k_thread_start(&led_thread);

	unsigned int cycle = 0;

	while (1) {
		printk("[console] cycle=%u, TX begin, char_wait_ms=%d\n",
		       ++cycle, UART_CHAR_WAIT_MS);
		uart_send_string("Hello World!\r\n");
		printk("[console] cycle=%u, TX loop ended, gap_ms=%d\n",
		       cycle, MESSAGE_GAP_MS);
		/* 次の送信までmainだけが待つ。LEDスレッドは点滅を続ける。 */
		k_msleep(MESSAGE_GAP_MS);
	}
}
