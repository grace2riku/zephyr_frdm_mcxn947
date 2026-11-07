/* SPDX-License-Identifier: Apache-2.0 */
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "memory_commands.h"
#include "serial_port.h"

#define TEST_RAM_SIZE 128U
#define DUMP_MAX_BYTES 64U

/* 4 B境界の専用領域。mainだけが操作する。GDBでもこの名前で照合する。 */
static uint32_t memory_test_buffer[TEST_RAM_SIZE / sizeof(uint32_t)];

static unsigned char *ram_bytes(void)
{
	return (unsigned char *)memory_test_buffer;
}

void memory_commands_init(void)
{
	for (size_t i = 0; i < TEST_RAM_SIZE; ++i) {
		ram_bytes()[i] = (unsigned char)i;
	}
}

void memory_commands_info(void)
{
	char line[112];
	uintptr_t base = (uintptr_t)memory_test_buffer;

	snprintf(line, sizeof(line),
		 "RAM: [0x%08" PRIxPTR ", 0x%08" PRIxPTR ") 128 bytes; end exclusive\r\n",
		 base, base + TEST_RAM_SIZE);
	serial_puts(line);
	serial_puts("read/write: 8/16/32 bit, aligned; dump: 1..64 bytes.\r\n");
}

void memory_commands_usage(void)
{
	serial_puts("Memory usage:\r\n"
		"  mem info\r\n"
		"  mem read <0xADDRESS> <8|16|32>\r\n"
		"  mem write <0xADDRESS> <8|16|32> <VALUE>\r\n"
		"  mem dump <0xADDRESS> <LENGTH:1..64>\r\n"
		"ADDRESS: hex with 0x; VALUE/LENGTH: decimal or 0x hex.\r\n"
		"Only the displayed test RAM range is accessible.\r\n");
}

/* strtoulの符号・空白・桁あふれの扱いに依存せず、乗算前に検査する。 */
static bool parse_number(const char *text, uintptr_t max, bool address,
			 uintptr_t *value)
{
	uintptr_t n = 0;
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
		if (digit >= radix || digit > max || n > (max - digit) / radix) {
			return false;
		}
		n = n * radix + digit;
	}
	*value = n;
	return true;
}

static bool parse_width(const char *text, size_t *bytes)
{
	/* 幅は表記も8/16/32に限定する。 */
	if (strcmp(text, "8") == 0) {
		*bytes = 1;
	} else if (strcmp(text, "16") == 0) {
		*bytes = 2;
	} else if (strcmp(text, "32") == 0) {
		*bytes = 4;
	} else {
		return false;
	}
	return true;
}

static bool checked_offset(uintptr_t address, size_t bytes, size_t alignment,
			   size_t *offset)
{
	uintptr_t base = (uintptr_t)memory_test_buffer;

	/* address + bytesは桁あふれ得るため使わない。検査前にポインタ化しない。 */
	if (address < base || address - base >= TEST_RAM_SIZE ||
	    bytes > TEST_RAM_SIZE - (address - base)) {
		serial_puts("ERROR: outside test RAM range\r\n");
		return false;
	}
	if (address % alignment != 0U) {
		serial_puts("ERROR: unaligned address\r\n");
		return false;
	}
	*offset = (size_t)(address - base);
	return true;
}

/* memcpyでオブジェクト表現を転送し、型の別名参照による未定義動作を避ける。
 * 幅は論理的な転送サイズ。生成されるロード/ストア命令の幅は保証しない。 */
static uint32_t read_value(size_t offset, size_t bytes)
{
	if (bytes == 1U) {
		return ram_bytes()[offset];
	}
	if (bytes == 2U) {
		uint16_t value;

		memcpy(&value, ram_bytes() + offset, sizeof(value));
		return value;
	}
	uint32_t value;

	memcpy(&value, ram_bytes() + offset, sizeof(value));
	return value;
}

static void write_value(size_t offset, size_t bytes, uint32_t value)
{
	if (bytes == 1U) {
		ram_bytes()[offset] = (unsigned char)value;
	} else if (bytes == 2U) {
		uint16_t half = (uint16_t)value;

		memcpy(ram_bytes() + offset, &half, sizeof(half));
	} else {
		memcpy(ram_bytes() + offset, &value, sizeof(value));
	}
}

static void dump_bytes(uintptr_t address, size_t offset, size_t length)
{
	for (size_t i = 0; i < length; i += 16U) {
		/* uintptr_tが64 bitの環境でも16バイトの行全体が収まる。 */
		char line[80];
		size_t used = (size_t)snprintf(line, sizeof(line), "0x%08" PRIxPTR ":",
					     address + i);

		for (size_t j = i; j < length && j < i + 16U; ++j) {
			used += (size_t)snprintf(line + used, sizeof(line) - used,
						" %02x", (unsigned int)ram_bytes()[offset + j]);
		}
		serial_puts(line);
		serial_puts("\r\n");
	}
}

void memory_commands_execute(int argc, char **argv)
{
	uintptr_t address;
	uintptr_t value = 0;
	size_t bytes;
	size_t offset;
	bool write;
	char line[96];

	if (argc == 2 && strcmp(argv[1], "info") == 0) {
		memory_commands_info();
		return;
	}
	if (argc == 4 && strcmp(argv[1], "dump") == 0) {
		uintptr_t length;

		if (!parse_number(argv[2], UINTPTR_MAX, true, &address) ||
		    !parse_number(argv[3], DUMP_MAX_BYTES, false, &length) || length == 0U) {
			goto invalid;
		}
		if (checked_offset(address, (size_t)length, 1U, &offset)) {
			dump_bytes(address, offset, (size_t)length);
		}
		return;
	}
	write = argc == 5 && strcmp(argv[1], "write") == 0;
	if (!write && !(argc == 4 && strcmp(argv[1], "read") == 0)) {
		goto invalid;
	}
	if (!parse_number(argv[2], UINTPTR_MAX, true, &address) ||
	    !parse_width(argv[3], &bytes)) {
		goto invalid;
	}
	if (write) {
		uintptr_t max = bytes == 4U ? UINT32_MAX : (1U << (bytes * 8U)) - 1U;

		if (!parse_number(argv[4], max, false, &value)) {
			goto invalid;
		}
	}
	if (!checked_offset(address, bytes, bytes, &offset)) {
		return;
	}
	if (write) {
		write_value(offset, bytes, (uint32_t)value);
	}
	snprintf(line, sizeof(line), "%s%u 0x%08" PRIxPTR " = 0x%0*" PRIx32 "\r\n",
		 write ? "W" : "R", (unsigned int)(bytes * 8U), address,
		 (int)(bytes * 2U), read_value(offset, bytes));
	serial_puts(line);
	return;

invalid:
	serial_puts("ERROR: invalid memory command or argument\r\n");
	memory_commands_usage();
}
