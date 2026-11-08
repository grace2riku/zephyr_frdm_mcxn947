/* SPDX-License-Identifier: Apache-2.0 */
#include <zephyr/console/console.h>
#include <zephyr/sys/printk.h>

int main(void)
{
	int ret = console_init();

	if (ret < 0) {
		printk("console_init failed: %d\n", ret);
		return ret;
	}

	printk("Stage 10: Console character echo\n");
	printk("Type one letter without Enter.\n");

	while (1) {
		int c = console_getchar();

		if (c < 0) {
			printk("console_getchar failed: %d\n", c);
			return c;
		}

		ret = console_putchar((char)c);
		if (ret < 0) {
			printk("console_putchar failed: %d\n", ret);
			return ret;
		}
	}
}
