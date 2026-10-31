/* SPDX-License-Identifier: Apache-2.0 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <zephyr/sys/util.h>

#include "ntopt.h"
#include "commands.h"
#include "led_control.h"
#include "serial_port.h"

static void usage(void)
{
	serial_puts("Usage:\r\n"
		"  help\r\n"
		"  led on\r\n"
		"  led off\r\n"
		"  led blink [100..2000]\r\n"
		"blink: toggle interval in ms; default 500; period = 2 x interval.\r\n");
}

/* 符号・単位・小数点を拒否し、乗算前に上限を検査する。 */
static bool parse_interval(const char *text, uint32_t *value)
{
	uint32_t n = 0;

	if (*text == '\0') {
		return false;
	}
	for (; *text != '\0'; ++text) {
		if (*text < '0' || *text > '9' ||
		    n > (LED_BLINK_MAX_MS - (uint32_t)(*text - '0')) / 10U) {
			return false;
		}
		n = n * 10U + (uint32_t)(*text - '0');
	}
	if (n < LED_BLINK_MIN_MS) {
		return false;
	}
	*value = n;
	return true;
}

static int dispatch(int argc, char **argv, void *context)
{
	enum led_mode mode;
	uint32_t interval_ms = LED_BLINK_DEFAULT_MS;

	ARG_UNUSED(context);
	if (argc == 0) {
		return 0;
	}
	if (argc == 1 && strcmp(argv[0], "help") == 0) {
		usage();
		return 0;
	}
	if (argc < 2 || strcmp(argv[0], "led") != 0) {
		goto invalid;
	}
	if (argc == 2 && strcmp(argv[1], "on") == 0) {
		mode = LED_ON;
	} else if (argc == 2 && strcmp(argv[1], "off") == 0) {
		mode = LED_OFF;
	} else if ((argc == 2 || argc == 3) && strcmp(argv[1], "blink") == 0) {
		mode = LED_BLINK;
		if (argc == 3 && !parse_interval(argv[2], &interval_ms)) {
			goto invalid;
		}
	} else {
		goto invalid;
	}
	if (led_control_apply(mode, interval_ms) < 0) {
		serial_puts("ERROR: LED request failed; actual LED state is unknown.\r\n");
		return 0;
	}
	serial_puts(mode == LED_ON ? "OK: led on\r\n" :
		mode == LED_OFF ? "OK: led off\r\n" : "OK: led blink\r\n");
	return 0;

invalid:
	serial_puts("ERROR: invalid command or argument\r\n");
	usage();
	return 0;
}

int command_callback(const char *text, void *context)
{
	ARG_UNUSED(context);
	if (serial_rx_failed()) {
		return 0; /* 欠落した可能性のあるコマンドは実行しない。 */
	}
	if (led_control_take_error() < 0) {
		serial_puts("ERROR: blink GPIO write failed; automatic toggling stopped.\r\n");
	}
	/* 上流は63 Bまで保持し、超過分を無視する。満杯行は解析・実行しない。
	 * 62 B以下ならntoptの単語別NULを含む64 Bにも収まる。 */
	if (strlen(text) >= NTOPT_TEXT_MAXLEN - 1U) {
		serial_puts("ERROR: command too long (max 62 bytes); discarded\r\n");
		usage();
		return 0;
	}
	return ntopt_parse(text, dispatch, NULL);
}
