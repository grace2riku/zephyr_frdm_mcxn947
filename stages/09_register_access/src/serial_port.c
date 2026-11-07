/* SPDX-License-Identifier: Apache-2.0 */
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>

#include "serial_port.h"

#define UART_NODE DT_CHOSEN(zephyr_console)
#if !DT_NODE_HAS_STATUS(UART_NODE, okay)
#error "zephyr,console must refer to an enabled UART node"
#endif

static const struct device *const uart_dev = DEVICE_DT_GET(UART_NODE);
/* 完成行ではなく、編集キーを含む受信バイトをコピーする。 */
K_MSGQ_DEFINE(rx_bytes, sizeof(uint8_t), 256, 1);
static atomic_t rx_failed;
static bool discard_line;
static bool cancel_pending;
static bool previous_was_cr;

static void record_errors(const struct device *dev)
{
	if (uart_err_check(dev) > 0) {
		atomic_set(&rx_failed, 1);
	}
}

static void serial_cb(const struct device *dev, void *context)
{
	uint8_t c;

	ARG_UNUSED(context);
	if (uart_irq_update(dev) <= 0) {
		return;
	}
	record_errors(dev);
	if (uart_irq_rx_ready(dev) <= 0) {
		return;
	}
	while (uart_fifo_read(dev, &c, 1) == 1) {
		record_errors(dev);
		if (k_msgq_put(&rx_bytes, &c, K_NO_WAIT) != 0) {
			atomic_set(&rx_failed, 1);
		}
	}
}

int serial_port_init(void)
{
	if (!device_is_ready(uart_dev)) {
		return -ENODEV;
	}
	if (uart_err_check(uart_dev) < 0) {
		return -ENOSYS;
	}
	return uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);
}

void serial_port_start(void)
{
	uart_irq_rx_enable(uart_dev);
	uart_irq_err_enable(uart_dev);
}

int serial_rx_failed(void)
{
	return atomic_get(&rx_failed) != 0;
}

int serial_write(const char *buf, int count, void *context)
{
	ARG_UNUSED(context);
	for (int i = 0; i < count; ++i) {
		uart_poll_out(uart_dev, (unsigned char)buf[i]);
	}
	return count;
}

void serial_puts(const char *text)
{
	serial_write(text, (int)strlen(text), NULL);
}

static uint8_t read_one(void)
{
	uint8_t c;

	while (1) {
		if (serial_rx_failed()) {
			/* CPU0上のISRと排他して、欠落を含む未処理入力を捨てる。 */
			unsigned int key = irq_lock();

			k_msgq_purge(&rx_bytes);
			atomic_clear(&rx_failed);
			irq_unlock(key);
			discard_line = true;
			previous_was_cr = false;
			cancel_pending = true;
			serial_puts("\r\nERROR: RX data lost; press Enter, then retype.\r\n");
			/* CANでVT解析状態を戻し、次のCtrl+Cで編集中の行を消す。 */
			return 0x18;
		}
		if (cancel_pending) {
			cancel_pending = false;
			return 0x03;
		}
		/* NT-Shellはreadの戻り値を検査しないため、必ず1バイト返す。 */
		if (k_msgq_get(&rx_bytes, &c, K_MSEC(100)) != 0) {
			continue;
		}
		if (serial_rx_failed()) {
			continue;
		}
		if (discard_line) {
			if (c == '\r' || c == '\n' || c == 0x03) {
				discard_line = false;
				previous_was_cr = (c == '\r');
				return 0x03;
			}
			continue;
		}
		if (c == '\n' && previous_was_cr) {
			previous_was_cr = false;
			continue;
		}
		previous_was_cr = (c == '\r');
		/* 上流はCRでEnter。LF単独もCRへ正規化する。 */
		return c == '\n' ? '\r' : c;
	}
}

int serial_read(char *buf, int count, void *context)
{
	ARG_UNUSED(context);
	for (int i = 0; i < count; ++i) {
		buf[i] = (char)read_one();
	}
	return count;
}
