/* SPDX-License-Identifier: Apache-2.0 */
#include <errno.h>
#include <stddef.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/sys_io.h>
#include <zephyr/sys/util.h>
#include <soc.h>
#include "register_access.h"

#define LED_NODE DT_ALIAS(led0)
#define GPIO_NODE DT_GPIO_CTLR(LED_NODE, gpios)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

BUILD_ASSERT(DT_SAME_NODE(GPIO_NODE, DT_NODELABEL(gpio0)));
BUILD_ASSERT(DT_GPIO_PIN(LED_NODE, gpios) == 10);
BUILD_ASSERT((DT_GPIO_FLAGS(LED_NODE, gpios) & GPIO_ACTIVE_LOW) != 0);
BUILD_ASSERT(DT_REG_ADDR(GPIO_NODE) == GPIO0_BASE);
BUILD_ASSERT(offsetof(GPIO_Type, PDOR) == REG_PDOR);
BUILD_ASSERT(offsetof(GPIO_Type, PSOR) == REG_PSOR);
BUILD_ASSERT(offsetof(GPIO_Type, PCOR) == REG_PCOR);
BUILD_ASSERT(offsetof(GPIO_Type, PTOR) == REG_PTOR);
BUILD_ASSERT(offsetof(GPIO_Type, PDIR) == REG_PDIR);
BUILD_ASSERT(offsetof(GPIO_Type, PDDR) == REG_PDDR);
BUILD_ASSERT(sizeof(((GPIO_Type *)0)->PDOR) == sizeof(uint32_t));
/* GPIOドライバは初期設定だけ。LEDドライバ・キー入力からの操作を防ぐ。 */
BUILD_ASSERT(!IS_ENABLED(CONFIG_LED_GPIO));
BUILD_ASSERT(!IS_ENABLED(CONFIG_INPUT_GPIO_KEYS));

int register_access_init(void)
{
	if (!gpio_is_ready_dt(&led)) {
		return -ENODEV;
	}
	/* 出力ラッチをHighにしてから出力へ切り替え、赤色LEDを消灯する。 */
	return gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
}

uintptr_t register_access_base(void)
{
	return DT_REG_ADDR(GPIO_NODE);
}

uint32_t register_access_read(uint32_t offset)
{
	return sys_read32(register_access_base() + offset);
}

void register_access_write(uint32_t offset, uint32_t value)
{
	/* W1S/W1C/toggleへの書き込みは単一store。read-modify-writeしない。 */
	sys_write32(value, register_access_base() + offset);
}
