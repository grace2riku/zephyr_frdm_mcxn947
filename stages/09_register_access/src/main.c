/* SPDX-License-Identifier: Apache-2.0 */
#include <stddef.h>
#include "ntshell.h"
#include "commands.h"
#include "memory_commands.h"
#include "register_commands.h"
#include "register_access.h"
#include "serial_port.h"

static ntshell_t shell;

int main(void)
{
	if (serial_port_init() < 0) {
		return 0;
	}
	if (register_access_init() < 0) {
		serial_puts("ERROR: GPIO setup failed; main exits.\r\n");
		return 0;
	}
	memory_commands_init();
	ntshell_init(&shell, serial_read, serial_write, command_callback, NULL);
	ntshell_set_prompt(&shell, "mcxn947> ");
	serial_puts("Stage 9: NT-Shell register commands\r\n"
		"NT-Shell package 0.3.1; local echo OFF; type help.\r\n"
		"Red LED starts OFF; GPIO0 bit 10 is owned by this shell.\r\n");
	register_commands_info();
	memory_commands_info();
	serial_port_start();
	ntshell_execute(&shell);
	return 0;
}
