/* SPDX-License-Identifier: Apache-2.0 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>

#define UART_NODE DT_CHOSEN(zephyr_console)

#if !DT_NODE_HAS_STATUS(UART_NODE, okay)
#error "zephyr,console must refer to an enabled UART node"
#endif

static const struct device *const uart_dev = DEVICE_DT_GET(UART_NODE);

/* printkを経由せず、指定したバイトを直接送る。終端NULは送らない。 */
static void uart_send_string(const char *text)
{
	while (*text != '\0') {
		uart_poll_out(uart_dev, (unsigned char)*text++);
	}
}

int main(void)
{
	if (!device_is_ready(uart_dev)) {
		/* UARTが未準備なら、このUARTへエラー表示することもできない。 */
		return 0;
	}

	uart_send_string("Stage 6: polling byte echo\r\n");
	uart_send_string("Local echo OFF; each received byte is returned unchanged.\r\n");

	while (1) {
		unsigned char c;
		int ret = uart_poll_in(uart_dev, &c);

		if (ret == 0) {
			/* 成功したときだけcを使う。CR/LFもそのまま返す。 */
			uart_poll_out(uart_dev, c);
		} else if (ret == -1) {
			/* API自身は受信を待たない。空ならmainを短時間休ませる。 */
			k_msleep(1);
		} else {
			uart_send_string("\r\nERROR: uart_poll_in failed; main exits.\r\n");
			return 0;
		}
	}
}
