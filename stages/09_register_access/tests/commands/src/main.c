/* SPDX-License-Identifier: Apache-2.0 */
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/ztest.h>
#include "commands.h"
#include "memory_commands.h"
#include "register_access.h"

/* 実際の解析・許可表・検証処理を使用。MMIOは副作用のあるモデルへ置換。 */
#define BASE UINT32_C(0x50096000)
static char output[4096];
static uint32_t pdor;
static uint32_t last_offset, last_value;
static unsigned int reads, writes;
static bool rx_fault, drop_write;

void serial_puts(const char *text)
{
	zassert_true(strlen(output) + strlen(text) < sizeof(output));
	strcat(output, text);
}

int serial_rx_failed(void)
{
	return rx_fault;
}

uintptr_t register_access_base(void)
{
	return BASE;
}

uint32_t register_access_read(uint32_t offset)
{
	reads++;
	zassert_true(offset == REG_PDOR || offset == REG_PDIR || offset == REG_PDDR,
		     "Forbidden read: 0x%x", offset);
	return offset == REG_PDDR ? REG_LED_MASK : pdor;
}

void register_access_write(uint32_t offset, uint32_t value)
{
	writes++;
	last_offset = offset;
	last_value = value;
	zassert_true(offset == REG_PSOR || offset == REG_PCOR || offset == REG_PTOR);
	zassert_equal(value & ~REG_LED_MASK, 0);
	if (drop_write) {
		return;
	}
	if (offset == REG_PSOR) {
		pdor |= value;
	} else if (offset == REG_PCOR) {
		pdor &= ~value;
	} else {
		pdor ^= value;
	}
}

static void reset(void *fixture)
{
	ARG_UNUSED(fixture);
	output[0] = '\0';
	reads = writes = 0;
	pdor = 0xa5a5045a;
	rx_fault = drop_write = false;
	memory_commands_init();
}

ZTEST(stage9_commands, test_info_help_and_empty_do_not_touch_mmio)
{
	command_callback("", NULL);
	command_callback("   ", NULL);
	zassert_equal(output[0], 0);
	command_callback("help", NULL);
	zassert_not_null(strstr(output, "reg write"));
	zassert_not_null(strstr(output, "mem read"));
	zassert_is_null(strstr(output, "led blink"));
	output[0] = '\0';
	command_callback("reg info", NULL);
	zassert_not_null(strstr(output, "PDOR 0x50096040 shell:read"));
	zassert_not_null(strstr(output, "PTOR 0x5009604c shell:write"));
	zassert_equal(reads + writes, 0);
}

ZTEST(stage9_commands, test_read_allowlist)
{
	command_callback("reg read 0x50096040 32", NULL);
	zassert_not_null(strstr(output, "R32 PDOR 0x50096040 = 0xa5a5045a"));
	command_callback("reg read 0x50096050 32", NULL);
	command_callback("reg read 0X50096054 32", NULL);
	zassert_not_null(strstr(output, "R32 PDDR 0x50096054 = 0x00000400"));
	zassert_equal(reads, 3);
	zassert_equal(writes, 0);
}

ZTEST(stage9_commands, test_write_side_effects_preserve_other_pins)
{
	command_callback("reg write 0x50096048 32 0x400", NULL);
	zassert_equal(pdor, 0xa5a5005a);
	zassert_equal(last_offset, REG_PCOR);
	zassert_equal(last_value, REG_LED_MASK);
	zassert_not_null(strstr(output, "PDOR.bit10=0 (LED ON)"));
	command_callback("reg write 0x50096044 32 1024", NULL);
	zassert_equal(pdor, 0xa5a5045a);
	command_callback("reg write 0x5009604c 32 0X400", NULL);
	zassert_equal(pdor, 0xa5a5005a);
	command_callback("reg write 0x5009604c 32 0x400", NULL);
	zassert_equal(pdor, 0xa5a5045a);
	zassert_equal(reads, 8);
	zassert_equal(writes, 4);
}

ZTEST(stage9_commands, test_zero_is_noop_and_repeated_set_clear_are_idempotent)
{
	const char *const zero[] = {
		"reg write 0x50096044 32 0", "reg write 0x50096048 32 0x0",
		"reg write 0x5009604c 32 0",
	};
	for (size_t i = 0; i < ARRAY_SIZE(zero); ++i) {
		command_callback(zero[i], NULL);
		zassert_equal(pdor, 0xa5a5045a);
	}
	command_callback("reg write 0x50096048 32 1024", NULL);
	command_callback("reg write 0x50096048 32 1024", NULL);
	zassert_equal(pdor, 0xa5a5005a);
	command_callback("reg write 0x50096044 32 1024", NULL);
	command_callback("reg write 0x50096044 32 1024", NULL);
	zassert_equal(pdor, 0xa5a5045a);
}

ZTEST(stage9_commands, test_denied_addresses_directions_and_bits_never_access_mmio)
{
	const char *const denied[] = {
		"reg read 0x50096044 32", "reg read 0x50096048 32",
		"reg read 0x5009604c 32", "reg write 0x50096040 32 0x400",
		"reg write 0x50096050 32 0", "reg write 0x50096054 32 0x400",
		"reg read 0x50096120 32", "reg write 0x50096120 32 0x400",
		"reg read 0x50096041 32", "reg read 0x50096000 32",
		"reg read 0x40096040 32", "reg read 0x00000000 32",
		"reg read 0xfffffffc 32", "reg read 0xffffffff 32",
		"reg write 0x50096044 32 1", "reg write 0x50096048 32 0x401",
		"reg write 0x5009604c 32 0xffffffff",
	};
	for (size_t i = 0; i < ARRAY_SIZE(denied); ++i) {
		output[0] = '\0';
		command_callback(denied[i], NULL);
		zassert_not_null(strstr(output, "ERROR:"), "%s", denied[i]);
		zassert_is_null(strstr(output, "OK:"));
		zassert_equal(reads + writes, 0, "%s touched MMIO", denied[i]);
		zassert_equal(pdor, 0xa5a5045a);
	}
}

ZTEST(stage9_commands, test_bad_syntax_and_numbers_never_access_mmio)
{
	const char *const bad[] = {
		"reg", "reg info extra", "reg read", "reg write", "reg dump",
		"reg read 0x50096040", "reg read 0x50096040 32 extra",
		"reg write 0x50096044 32", "reg write 0x50096044 32 0 extra",
		"reg read 0x50096040 8", "reg read 0x50096040 16",
		"reg read 0x50096040 032", "reg read 0x50096040 0x20",
		"reg read 50096040 32", "reg read -0x50096040 32",
		"reg read 0x 32", "reg read 0x5009604g 32",
		"reg read 0x150096040 32", "reg read 0x100000000 32",
		"reg write 0x50096044 32 -1", "reg write 0x50096044 32 +0",
		"reg write 0x50096044 32 1.0", "reg write 0x50096044 32 0x",
		"reg write 0x50096044 32 4294967296",
		"reg write 0x50096044 32 0x100000000",
		"reg write 0x50096044 32 99999999999999999999999999999",
		"led on", "led blink", "unknown", "help extra",
	};
	for (size_t i = 0; i < ARRAY_SIZE(bad); ++i) {
		output[0] = '\0';
		command_callback(bad[i], NULL);
		zassert_not_null(strstr(output, "ERROR:"), "%s", bad[i]);
		zassert_equal(reads + writes, 0, "%s touched MMIO", bad[i]);
	}
}

ZTEST(stage9_commands, test_fault_and_full_line_block_write_and_recover)
{
	char line[64];
	const char *cmd = "reg write 0x50096048 32 0x400";

	rx_fault = true;
	command_callback(cmd, NULL);
	zassert_equal(reads + writes, 0);
	rx_fault = false;
	memset(line, ' ', sizeof(line));
	memcpy(line, cmd, strlen(cmd));
	line[63] = '\0';
	command_callback(line, NULL);
	zassert_equal(reads + writes, 0);
	zassert_not_null(strstr(output, "command too long"));
	line[62] = '\0';
	command_callback(line, NULL);
	zassert_equal(writes, 1);
	zassert_equal(pdor, 0xa5a5005a);
}

ZTEST(stage9_commands, test_failed_latch_check_has_no_success_reply)
{
	drop_write = true;
	command_callback("reg write 0x50096048 32 0x400", NULL);
	zassert_not_null(strstr(output, "verification failed"));
	zassert_is_null(strstr(output, "OK:"));
	zassert_equal(pdor, 0xa5a5045a);
}

ZTEST(stage9_commands, test_ram_remains_separate_from_register_access)
{
	uintptr_t ram_base;
	char cmd[64];

	command_callback("mem info", NULL);
	zassert_equal(sscanf(output, "RAM: [0x%" SCNxPTR, &ram_base), 1);
	output[0] = '\0';
	snprintf(cmd, sizeof(cmd), "mem write 0x%" PRIxPTR " 32 0x12345678", ram_base);
	command_callback(cmd, NULL);
	snprintf(cmd, sizeof(cmd), "mem read 0x%" PRIxPTR " 32", ram_base);
	command_callback(cmd, NULL);
	zassert_not_null(strstr(output, "0x12345678"));
	output[0] = '\0';
	command_callback("mem write 0x50096044 32 0x400", NULL);
	zassert_not_null(strstr(output, "outside test RAM range"));
	snprintf(cmd, sizeof(cmd), "reg read 0x%" PRIxPTR " 32", ram_base);
	command_callback(cmd, NULL);
	zassert_not_null(strstr(output, "not permitted"));
	zassert_equal(reads + writes, 0);
}

ZTEST_SUITE(stage9_commands, NULL, NULL, reset, NULL, NULL);
