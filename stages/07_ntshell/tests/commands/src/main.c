/* SPDX-License-Identifier: Apache-2.0 */
#include <errno.h>
#include <string.h>
#include <zephyr/ztest.h>
#include "commands.h"
#include "led_control.h"

/* UARTとGPIOの代わりに呼出しを記録し、本物のcommands.c/ntopt.cを検査。 */
static char output[2048];
static unsigned int requests;
static enum led_mode last_mode;
static uint32_t last_interval;
static int gpio_result;
static int rx_fault;
static int blink_fault;

void serial_puts(const char *text)
{
	zassert_true(strlen(output) + strlen(text) < sizeof(output));
	strcat(output, text);
}

int serial_rx_failed(void)
{
	return rx_fault;
}

int led_control_apply(enum led_mode mode, uint32_t interval_ms)
{
	requests++;
	last_mode = mode;
	last_interval = interval_ms;
	return gpio_result;
}

int led_control_take_error(void)
{
	int error = blink_fault;

	blink_fault = 0;
	return error;
}

static void reset(void *fixture)
{
	ARG_UNUSED(fixture);
	output[0] = '\0';
	requests = 0;
	gpio_result = 0;
	rx_fault = 0;
	blink_fault = 0;
}

ZTEST(stage7_commands, test_help_and_empty)
{
	command_callback("", NULL);
	command_callback("   ", NULL);
	zassert_equal(output[0], '\0');
	command_callback("help", NULL);
	zassert_not_null(strstr(output, "Usage:"));
	zassert_not_null(strstr(output, "led blink"));
	zassert_equal(requests, 0);
}

ZTEST(stage7_commands, test_valid_led_commands)
{
	command_callback("led on", NULL);
	zassert_equal(last_mode, LED_ON);
	command_callback("  led   off  ", NULL);
	zassert_equal(last_mode, LED_OFF);
	command_callback("led blink", NULL);
	zassert_equal(last_mode, LED_BLINK);
	zassert_equal(last_interval, 500);
	command_callback("led blink 100", NULL);
	zassert_equal(last_interval, 100);
	command_callback("led blink 2000", NULL);
	zassert_equal(last_interval, 2000);
	zassert_equal(requests, 5);
}

ZTEST(stage7_commands, test_invalid_input_never_changes_led)
{
	const char *const invalid[] = {
		"unknown", "help extra", "led", "led ON", "led toggle",
		"led on extra", "led off extra", "led blink 100 extra",
		"led blink 0", "led blink 99", "led blink 2001",
		"led blink -100", "led blink +100", "led blink 100ms",
		"led blink 1.0", "led blink 4294967296",
		"led blink 999999999999999999999999999999999999999",
	};

	for (size_t i = 0; i < ARRAY_SIZE(invalid); ++i) {
		output[0] = '\0';
		command_callback(invalid[i], NULL);
		zassert_not_null(strstr(output, "ERROR:"), "%s", invalid[i]);
		zassert_not_null(strstr(output, "Usage:"), "%s", invalid[i]);
		zassert_is_null(strstr(output, "OK:"));
	}
	zassert_equal(requests, 0);
}

ZTEST(stage7_commands, test_length_boundary_and_many_arguments)
{
	char line[64];

	memset(line, ' ', sizeof(line));
	memcpy(line, "led on", 6);
	line[62] = '\0';
	command_callback(line, NULL);
	zassert_equal(requests, 1); /* 62 Bは切り捨てられていない。 */
	line[62] = ' ';
	line[63] = '\0';
	command_callback(line, NULL);
	zassert_equal(requests, 1); /* 満杯の63 Bは有効な先頭があっても拒否。 */
	zassert_not_null(strstr(output, "command too long"));
	output[0] = '\0';
	for (size_t i = 0; i < 62; ++i) {
		line[i] = (i % 2 == 0) ? 'x' : ' ';
	}
	line[62] = '\0';
	command_callback(line, NULL);
	zassert_equal(requests, 1);
	zassert_not_null(strstr(output, "Usage:"));
}

ZTEST(stage7_commands, test_rx_loss_blocks_execution)
{
	rx_fault = 1;
	command_callback("led on", NULL);
	zassert_equal(requests, 0);
	rx_fault = 0;
	command_callback("led off", NULL);
	zassert_equal(requests, 1);
}

ZTEST(stage7_commands, test_gpio_error_has_no_success_reply)
{
	gpio_result = -EIO;
	command_callback("led on", NULL);
	zassert_not_null(strstr(output, "LED request failed"));
	zassert_is_null(strstr(output, "OK:"));
}

ZTEST(stage7_commands, test_blink_error_reported_before_next_command)
{
	blink_fault = -EIO;
	command_callback("help", NULL);
	zassert_not_null(strstr(output, "automatic toggling stopped"));
	zassert_not_null(strstr(output, "Usage:"));
	zassert_equal(blink_fault, 0);
}

ZTEST_SUITE(stage7_commands, NULL, NULL, reset, NULL, NULL);
