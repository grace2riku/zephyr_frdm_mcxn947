/* SPDX-License-Identifier: Apache-2.0 */
#ifndef STAGE7_SERIAL_PORT_H
#define STAGE7_SERIAL_PORT_H

int serial_port_init(void);
void serial_port_start(void);
int serial_read(char *buf, int count, void *context);
int serial_write(const char *buf, int count, void *context);
void serial_puts(const char *text);
int serial_rx_failed(void);

#endif
