/* SPDX-License-Identifier: Apache-2.0 */
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/sys/util.h>
#include "register_commands.h"
#include "register_access.h"
#include "serial_port.h"

enum access { READ_ONLY, SET, CLEAR, TOGGLE };
static const struct register_spec {
	const char *name;
	uint32_t offset;
	enum access access;
} registers[] = {
	{"PDOR", REG_PDOR, READ_ONLY},
	{"PSOR", REG_PSOR, SET},
	{"PCOR", REG_PCOR, CLEAR},
	{"PTOR", REG_PTOR, TOGGLE},
	{"PDIR", REG_PDIR, READ_ONLY},
	{"PDDR", REG_PDDR, READ_ONLY},
};

void register_commands_usage(void)
{
	serial_puts("Register usage:\r\n"
		"  reg info\r\n"
		"  reg read <0xADDRESS> 32\r\n"
		"  reg write <0xADDRESS> 32 <VALUE>\r\n"
		"Read: PDOR/PDIR/PDDR only. Write: PSOR/PCOR/PTOR only.\r\n"
		"VALUE: 0 or 0x400 (GPIO0 bit 10); decimal or 0x hex.\r\n");
}

void register_commands_info(void)
{
	char line[112];

	serial_puts("GPIO0: 32-bit only; red LED bit 10, active low; mask=0x00000400\r\n");
	for (size_t i = 0; i < ARRAY_SIZE(registers); ++i) {
		snprintf(line, sizeof(line), "  %s 0x%08" PRIxPTR " shell:%s\r\n",
			 registers[i].name, register_access_base() + registers[i].offset,
			 registers[i].access == READ_ONLY ? "read" : "write");
		serial_puts(line);
	}
	serial_puts("Writes are verified with PDOR bit 10; no LED blink task.\r\n");
}

static bool parse_number(const char *text, bool address, uint32_t *value)
{
	uint32_t n = 0;
	unsigned int radix = 10;

	if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
		radix = 16;
		text += 2;
	} else if (address) {
		return false;
	}
	if (*text == '\0') {
		return false;
	}
	for (; *text != '\0'; ++text) {
		unsigned int digit;

		if (*text >= '0' && *text <= '9') {
			digit = (unsigned int)(*text - '0');
		} else if (*text >= 'a' && *text <= 'f') {
			digit = (unsigned int)(*text - 'a') + 10U;
		} else if (*text >= 'A' && *text <= 'F') {
			digit = (unsigned int)(*text - 'A') + 10U;
		} else {
			return false;
		}
		if (digit >= radix || n > (UINT32_MAX - digit) / radix) {
			return false;
		}
		n = n * radix + digit;
	}
	*value = n;
	return true;
}

void register_commands_execute(int argc, char **argv)
{
	const struct register_spec *spec = NULL;
	uint32_t address, value = 0, before, after, expected;
	bool write;
	char line[128];

	if (argc == 2 && strcmp(argv[1], "info") == 0) {
		register_commands_info();
		return;
	}
	write = argc == 5 && strcmp(argv[1], "write") == 0;
	if ((!write && !(argc == 4 && strcmp(argv[1], "read") == 0)) ||
	    strcmp(argv[3], "32") != 0 ||
	    !parse_number(argv[2], true, &address) ||
	    (write && !parse_number(argv[4], false, &value))) {
		serial_puts("ERROR: invalid register command, number or width\r\n");
		register_commands_usage();
		return;
	}
	/* 任意ポインタを作らず、整列済みの許可表のアドレスと完全一致させる。 */
	for (size_t i = 0; i < ARRAY_SIZE(registers); ++i) {
		if ((uintptr_t)address == register_access_base() + registers[i].offset) {
			spec = &registers[i];
			break;
		}
	}
	if (spec == NULL || (write && spec->access == READ_ONLY) ||
	    (!write && spec->access != READ_ONLY)) {
		serial_puts("ERROR: address or access not permitted; see reg info\r\n");
		return;
	}
	if (!write) {
		value = register_access_read(spec->offset);
		snprintf(line, sizeof(line), "R32 %s 0x%08" PRIx32 " = 0x%08" PRIx32 "\r\n",
			 spec->name, address, value);
		serial_puts(line);
		return;
	}
	if ((value & ~REG_LED_MASK) != 0U) {
		serial_puts("ERROR: write value contains bits outside 0x00000400\r\n");
		return;
	}
	before = register_access_read(REG_PDOR);
	expected = spec->access == SET ? before | value :
		   spec->access == CLEAR ? before & ~value : before ^ value;
	register_access_write(spec->offset, value);
	after = register_access_read(REG_PDOR);
	if ((after & REG_LED_MASK) != (expected & REG_LED_MASK)) {
		serial_puts("ERROR: PDOR bit 10 verification failed; LED state unknown\r\n");
		return;
	}
	snprintf(line, sizeof(line),
		 "OK: W32 %s value=0x%08" PRIx32 " PDOR.bit10=%u (LED %s)\r\n",
		 spec->name, value, (after & REG_LED_MASK) != 0U,
		 (after & REG_LED_MASK) != 0U ? "OFF" : "ON");
	serial_puts(line);
}
