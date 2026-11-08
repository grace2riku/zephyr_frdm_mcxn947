/* SPDX-License-Identifier: Apache-2.0 */
#include <errno.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/util.h>

#include "led_control.h"

static int apply_led(const struct shell *sh, enum led_mode mode, uint32_t interval_ms)
{
	int ret = led_control_take_error();

	if (ret < 0) {
		shell_error(sh, "Previous blink stopped after GPIO error: %d", ret);
	}

	ret = led_control_apply(mode, interval_ms);
	if (ret < 0) {
		shell_error(sh, "LED request failed: %d", ret);
		return ret;
	}

	if (mode == LED_BLINK) {
		shell_print(sh, "LED BLINK toggle_ms=%u", (unsigned int)interval_ms);
	} else {
		shell_print(sh, "LED %s", mode == LED_ON ? "ON" : "OFF");
	}
	return 0;
}

static int cmd_led_on(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);
	return apply_led(sh, LED_ON, LED_BLINK_DEFAULT_MS);
}

static int cmd_led_off(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);
	return apply_led(sh, LED_OFF, LED_BLINK_DEFAULT_MS);
}

static int cmd_led_blink(const struct shell *sh, size_t argc, char **argv)
{
	int err = 0;
	unsigned long interval_ms = shell_strtoul(argv[1], 10, &err);

	ARG_UNUSED(argc);
	if (err != 0 || interval_ms < LED_BLINK_MIN_MS || interval_ms > LED_BLINK_MAX_MS) {
		shell_error(sh, "Use: led blink <100..2000 ms>");
		return -EINVAL;
	}
	return apply_led(sh, LED_BLINK, (uint32_t)interval_ms);
}

SHELL_STATIC_SUBCMD_SET_CREATE(led_commands,
	SHELL_CMD_ARG(on, NULL, "Turn the red LED on.", cmd_led_on, 1, 0),
	SHELL_CMD_ARG(off, NULL, "Turn the red LED off.", cmd_led_off, 1, 0),
	SHELL_CMD_ARG(blink, NULL, "Blink: toggle interval in ms (100..2000).",
		      cmd_led_blink, 2, 0),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(led, &led_commands, "Control the red LED.", NULL);

static int cmd_hello(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "Hello from Zephyr Shell.");
	return 0;
}

SHELL_CMD_ARG_REGISTER(hello, NULL, "Print a greeting.", cmd_hello, 1, 0);

int main(void)
{
	/* Shell runs in its own thread. */
	return 0;
}
