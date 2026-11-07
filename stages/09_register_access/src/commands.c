/* SPDX-License-Identifier: Apache-2.0 */
#include <string.h>
#include <zephyr/sys/util.h>
#include "ntopt.h"
#include "commands.h"
#include "memory_commands.h"
#include "register_commands.h"
#include "serial_port.h"

static void usage(void)
{
	serial_puts("Usage:\r\n  help\r\n");
	register_commands_usage();
	memory_commands_usage();
}

static int dispatch(int argc, char **argv, void *context)
{
	ARG_UNUSED(context);
	if (argc == 0) {
		return 0;
	}
	if (argc == 1 && strcmp(argv[0], "help") == 0) {
		usage();
	} else if (strcmp(argv[0], "reg") == 0) {
		register_commands_execute(argc, argv);
	} else if (strcmp(argv[0], "mem") == 0) {
		memory_commands_execute(argc, argv);
	} else {
		serial_puts("ERROR: invalid command or argument\r\n");
		usage();
	}
	return 0;
}

int command_callback(const char *text, void *context)
{
	ARG_UNUSED(context);
	if (serial_rx_failed()) {
		return 0;
	}
	/* 上流は63 Bを超える文字を無視するため、満杯行は実行しない。 */
	if (strlen(text) >= NTOPT_TEXT_MAXLEN - 1U) {
		serial_puts("ERROR: command too long (max 62 bytes); discarded\r\n");
		usage();
		return 0;
	}
	return ntopt_parse(text, dispatch, NULL);
}
