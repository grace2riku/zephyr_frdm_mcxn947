/* SPDX-License-Identifier: Apache-2.0 */
#include <stddef.h>
#include "ntshell.h"
#include "commands.h"
#include "led_control.h"
#include "serial_port.h"

/* 編集・履歴用の領域をmainのスタックに積まない。 */
static ntshell_t shell;

int main(void)
{
	if (serial_port_init() < 0) {
		return 0;
	}
	if (led_control_init() < 0) {
		serial_puts("ERROR: LED setup failed; main exits.\r\n");
		return 0;
	}
	ntshell_init(&shell, serial_read, serial_write, command_callback, NULL);
	ntshell_set_prompt(&shell, "mcxn947> ");
	serial_puts("Stage 7: NT-Shell LED commands\r\n"
		"NT-Shell package 0.3.1; local echo OFF; type help.\r\n"
		"LED starts OFF; blink default toggle interval=500 ms.\r\n");
	serial_port_start();
	ntshell_execute(&shell);
	return 0;
}
