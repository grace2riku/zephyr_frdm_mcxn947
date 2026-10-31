/* SPDX-License-Identifier: Apache-2.0 */
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/ztest.h>
#include "commands.h"
#include "led_control.h"
#include "memory_commands.h"

/* UARTとGPIOの代わりに呼出しを記録し、本物のcommands.c/ntopt.cを検査。 */
static char output[2048];
static unsigned int requests;
static enum led_mode last_mode;
static uint32_t last_interval;
static int gpio_result;
static int rx_fault;
static int blink_fault;
static uintptr_t ram_base;

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
	memory_commands_init();
	memory_commands_info();
	zassert_equal(sscanf(output, "RAM: [0x%" SCNxPTR, &ram_base), 1);
	output[0] = '\0';
}

ZTEST(stage8_commands, test_help_and_empty)
{
	command_callback("", NULL);
	command_callback("   ", NULL);
	zassert_equal(output[0], '\0');
	command_callback("help", NULL);
	zassert_not_null(strstr(output, "Usage:"));
	zassert_not_null(strstr(output, "led blink"));
	zassert_equal(requests, 0);
}

ZTEST(stage8_commands, test_valid_led_commands)
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

ZTEST(stage8_commands, test_invalid_input_never_changes_led)
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

ZTEST(stage8_commands, test_length_boundary_and_many_arguments)
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

ZTEST(stage8_commands, test_rx_loss_blocks_execution)
{
	rx_fault = 1;
	command_callback("led on", NULL);
	zassert_equal(requests, 0);
	rx_fault = 0;
	command_callback("led off", NULL);
	zassert_equal(requests, 1);
}

ZTEST(stage8_commands, test_gpio_error_has_no_success_reply)
{
	gpio_result = -EIO;
	command_callback("led on", NULL);
	zassert_not_null(strstr(output, "LED request failed"));
	zassert_is_null(strstr(output, "OK:"));
}

ZTEST(stage8_commands, test_blink_error_reported_before_next_command)
{
	blink_fault = -EIO;
	command_callback("help", NULL);
	zassert_not_null(strstr(output, "automatic toggling stopped"));
	zassert_not_null(strstr(output, "Usage:"));
	zassert_equal(blink_fault, 0);
}

static void memory_command(const char *operation, uintptr_t address, const char *args)
{
	char command[96];

	output[0] = '\0';
	snprintf(command, sizeof(command), "mem %s 0x%" PRIxPTR " %s",
		 operation, address, args);
	command_callback(command, NULL);
}

ZTEST(stage8_commands, test_ram_pattern_and_info)
{
	const unsigned char *ram = (const unsigned char *)ram_base;

	for (size_t i = 0; i < 128; ++i) {
		zassert_equal(ram[i], i);
	}
	command_callback("mem info", NULL);
	zassert_not_null(strstr(output, "128 bytes; end exclusive"));
	memory_command("read", ram_base, "8");
	zassert_not_null(strstr(output, " = 0x00\r\n"));
	memory_command("read", ram_base, "16");
	zassert_not_null(strstr(output, " = 0x0100\r\n"));
	memory_command("read", ram_base, "32");
	zassert_not_null(strstr(output, " = 0x03020100\r\n"));
}

ZTEST(stage8_commands, test_write_widths_and_dump)
{
	const unsigned char *ram = (const unsigned char *)ram_base;
	const unsigned char expected[] = {0x78, 0x56, 0x34, 0x12, 0xcd, 0xab, 0xff, 7};

	memory_command("write", ram_base, "32 0x12345678");
	zassert_not_null(strstr(output, "W32 "));
	zassert_not_null(strstr(output, " = 0x12345678"));
	memory_command("write", ram_base + 4, "16 0xabcd");
	memory_command("write", ram_base + 6, "8 255");
	zassert_mem_equal(ram, expected, sizeof(expected));
	memory_command("read", ram_base, "32");
	zassert_not_null(strstr(output, " = 0x12345678"));
	memory_command("read", ram_base + 4, "16");
	zassert_not_null(strstr(output, " = 0xabcd"));
	memory_command("read", ram_base + 6, "8");
	zassert_not_null(strstr(output, " = 0xff"));
	memory_command("dump", ram_base, "8");
	zassert_not_null(strstr(output, ": 78 56 34 12 cd ab ff 07\r\n"));
	zassert_equal(requests, 0);
}

ZTEST(stage8_commands, test_last_valid_access_and_max_values)
{
	const struct { size_t offset; const char *args; const char *result; } cases[] = {
		{127, "8 0xff", " = 0xff"},
		{126, "16 65535", " = 0xffff"},
		{124, "32 4294967295", " = 0xffffffff"},
	};

	for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
		memory_command("write", ram_base + cases[i].offset, cases[i].args);
		zassert_is_null(strstr(output, "ERROR:"));
		zassert_not_null(strstr(output, cases[i].result));
	}
	memory_command("write", ram_base, "32 0");
	zassert_not_null(strstr(output, " = 0x00000000"));
	memory_command("dump", ram_base + 127, "1");
	zassert_not_null(strstr(output, ": ff\r\n"));
}

ZTEST(stage8_commands, test_bad_ranges_never_access_memory)
{
	unsigned char before[128];
	const uintptr_t addresses[] = {0, ram_base - 1, ram_base + 128,
		UINTPTR_MAX, UINTPTR_MAX - 3};

	memcpy(before, (const void *)ram_base, sizeof(before));
	for (size_t i = 0; i < ARRAY_SIZE(addresses); ++i) {
		memory_command("write", addresses[i], "8 1");
		zassert_not_null(strstr(output, "outside test RAM"));
		memory_command("read", addresses[i], "32");
		zassert_not_null(strstr(output, "outside test RAM"));
		memory_command("dump", addresses[i], "1");
		zassert_not_null(strstr(output, "outside test RAM"));
	}
	memory_command("dump", ram_base + 127, "2");
	zassert_not_null(strstr(output, "outside test RAM"));
	memory_command("dump", ram_base + 65, "64");
	zassert_not_null(strstr(output, "outside test RAM"));
	memory_command("write", ram_base + 126, "32 1");
	zassert_not_null(strstr(output, "outside test RAM"));
	zassert_mem_equal(before, (const void *)ram_base, sizeof(before));
}

ZTEST(stage8_commands, test_unaligned_access_rejected)
{
	unsigned char before[128];

	memcpy(before, (const void *)ram_base, sizeof(before));
	for (size_t i = 1; i <= 3; ++i) {
		memory_command("read", ram_base + i, "32");
		zassert_not_null(strstr(output, "unaligned"));
		memory_command("write", ram_base + i, "32 0");
		zassert_not_null(strstr(output, "unaligned"));
	}
	memory_command("write", ram_base + 1, "16 0");
	zassert_not_null(strstr(output, "unaligned"));
	memory_command("read", ram_base + 1, "16");
	zassert_not_null(strstr(output, "unaligned"));
	zassert_mem_equal(before, (const void *)ram_base, sizeof(before));
	memory_command("dump", ram_base + 1, "1");
	zassert_not_null(strstr(output, ": 01\r\n"));
}

ZTEST(stage8_commands, test_bad_numbers_widths_and_lengths)
{
	unsigned char before[128];
	const char *const write_args[] = {
		"8 256", "16 65536", "32 4294967296", "8 -1", "8 +1",
		"8 0x", "8 0xgg", "8 1.0", "8 1x", "8 0x100",
		"8 999999999999999999999999", "64 1", "0 1", "0x8 1",
		"08 1", "8 1 extra", "8", "8 0X100",
	};
	const char *const lengths[] = {"0", "65", "0x41", "-1", "+1", "1 extra",
		"4294967296", "0x", "0xffffffffffffffffffffffff"};
	const char *const commands[] = {"mem", "mem info extra", "mem unknown",
		"mem read 0x 8", "mem read 1234 8", "mem read 0xG 8",
		"mem read 0x100000000 8", "mem read -0x1 8", "mem read 0x1 8 extra"};

	memcpy(before, (const void *)ram_base, sizeof(before));
	for (size_t i = 0; i < ARRAY_SIZE(write_args); ++i) {
		memory_command("write", ram_base, write_args[i]);
		zassert_not_null(strstr(output, "ERROR:"), "%s", write_args[i]);
	}
	for (size_t i = 0; i < ARRAY_SIZE(lengths); ++i) {
		memory_command("dump", ram_base, lengths[i]);
		zassert_not_null(strstr(output, "ERROR:"), "%s", lengths[i]);
	}
	for (size_t i = 0; i < ARRAY_SIZE(commands); ++i) {
		output[0] = '\0';
		command_callback(commands[i], NULL);
		zassert_not_null(strstr(output, "ERROR:"), "%s", commands[i]);
	}
	zassert_mem_equal(before, (const void *)ram_base, sizeof(before));
}

ZTEST(stage8_commands, test_dump_maximum_and_partial_row)
{
	memory_command("dump", ram_base, "0x40");
	zassert_is_null(strstr(output, "ERROR:"));
	zassert_not_null(strstr(output, ": 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\r\n"));
	zassert_not_null(strstr(output, ": 30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f\r\n"));
	zassert_equal(strchr(output, '\n') != NULL, true);
	zassert_is_null(strstr(output, " 40"));
	memory_command("dump", ram_base + 111, "17");
	zassert_not_null(strstr(output, ": 7f\r\n"));
	zassert_is_null(strstr(output, " 80"));
}

ZTEST(stage8_commands, test_fault_and_long_line_block_ram_write)
{
	unsigned char before[128];
	char line[64];

	memcpy(before, (const void *)ram_base, sizeof(before));
	rx_fault = 1;
	memory_command("write", ram_base, "32 0");
	zassert_equal(output[0], '\0');
	rx_fault = 0;
	memset(line, ' ', sizeof(line));
	snprintf(line, sizeof(line), "mem write 0x%" PRIxPTR " 32 0", ram_base);
	line[strlen(line)] = ' ';
	line[63] = '\0';
	output[0] = '\0';
	command_callback(line, NULL);
	zassert_not_null(strstr(output, "command too long"));
	zassert_mem_equal(before, (const void *)ram_base, sizeof(before));
	/* 拒否後の有効なコマンドは通常どおり実行される。 */
	memory_command("write", ram_base, "32 0X12345678");
	zassert_not_null(strstr(output, " = 0x12345678"));
}

ZTEST_SUITE(stage8_commands, NULL, NULL, reset, NULL, NULL);
