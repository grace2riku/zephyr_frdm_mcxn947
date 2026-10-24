/* SPDX-License-Identifier: Apache-2.0 */

#include <stdbool.h>
#include <stddef.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>

#define UART_NODE DT_CHOSEN(zephyr_console)
#define LINE_BUFFER_SIZE 32

#if !DT_NODE_HAS_STATUS(UART_NODE, okay)
#error "zephyr,console must refer to an enabled UART node"
#endif

static const struct device *const uart_dev = DEVICE_DT_GET(UART_NODE);

enum line_error {
	LINE_OK,
	LINE_TOO_LONG,
	LINE_INVALID_BYTE,
	LINE_UART_ERROR,
};

/* 受信状態の読み書きはmainだけが行う。1 Bは終端NUL用に残す。 */
static char line_buf[LINE_BUFFER_SIZE];
static size_t line_len;
static enum line_error pending_error;
static bool previous_was_cr;
static int rx_error_flags;

static void uart_send_string(const char *text)
{
	while (*text != '\0') {
		uart_poll_out(uart_dev, (unsigned char)*text++);
	}
}

static bool check_rx_errors(void)
{
	/* 採用MCUXドライバはエラーフラグの取得と解除をここで行う。 */
	int flags = uart_err_check(uart_dev);

	if (flags < 0) {
		uart_send_string("\r\nERROR: uart_err_check failed; main exits.\r\n");
		return false;
	}
	if (flags != 0) {
		/* 欠落の可能性がある行は返さず、次の行終端まで捨てる。 */
		rx_error_flags |= flags;
		pending_error = LINE_UART_ERROR;
		line_len = 0;
		previous_was_cr = false;
	}
	return true;
}

static void process_byte(unsigned char c)
{
	/* CRLFのLFだけを省く。CR単独・LF単独はそれぞれ行の終端。 */
	if (c == '\n' && previous_was_cr) {
		previous_was_cr = false;
		return;
	}
	previous_was_cr = (c == '\r');

	if (c == '\r' || c == '\n') {
		if (pending_error == LINE_UART_ERROR) {
			if (rx_error_flags & UART_ERROR_OVERRUN) {
				uart_send_string("ERROR: UART RX overrun; line discarded\r\n");
			} else {
				uart_send_string("ERROR: UART RX error; line discarded\r\n");
			}
		} else if (pending_error == LINE_TOO_LONG) {
			uart_send_string("ERROR: line too long (max 31 bytes); discarded\r\n");
		} else if (pending_error == LINE_INVALID_BYTE) {
			uart_send_string("ERROR: printable ASCII only; line discarded\r\n");
		} else {
			line_buf[line_len] = '\0';
			uart_send_string("Echo: ");
			uart_send_string(line_buf);
			uart_send_string("\r\n");
		}
		line_len = 0;
		pending_error = LINE_OK;
		rx_error_flags = 0;
		return;
	}

	/* 異常を検出した行は、終端が届くまで残りを捨てる。 */
	if (pending_error != LINE_OK) {
		return;
	}
	if (c < 0x20 || c > 0x7e) {
		/* NULやBackspaceも受け付けず、文字列の途中終了を防ぐ。 */
		pending_error = LINE_INVALID_BYTE;
		return;
	}
	if (line_len >= sizeof(line_buf) - 1) {
		pending_error = LINE_TOO_LONG;
		return;
	}
	line_buf[line_len++] = (char)c;
}

int main(void)
{
	if (!device_is_ready(uart_dev)) {
		return 0;
	}

	uart_send_string("Stage 6: polling line echo\r\n");
	uart_send_string("Local echo OFF; max 31 printable ASCII bytes; CR/LF ends a line.\r\n");
	uart_send_string("RX error recovery enabled.\r\n");

	while (1) {
		unsigned char c;
		int ret;

		if (!check_rx_errors()) {
			return 0;
		}
		ret = uart_poll_in(uart_dev, &c);

		if (ret == 0) {
			/* 直前の検査と読出しの間に発生したエラーも確認する。 */
			if (!check_rx_errors()) {
				return 0;
			}
			process_byte(c);
		} else if (ret == -1) {
			k_msleep(1);
		} else {
			uart_send_string("\r\nERROR: uart_poll_in failed; main exits.\r\n");
			return 0;
		}
	}
}
