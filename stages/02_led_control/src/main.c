/* SPDX-License-Identifier: Apache-2.0 */

#include <errno.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

/* 実習で1（点灯）から0（消灯）へ変更する。論理値であり、電圧ではない。 */
#define LED_STATE 0

#if (LED_STATE != 0) && (LED_STATE != 1)
#error "LED_STATE must be 0 or 1"
#endif

#define LED_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(LED_NODE, okay)
#error "led0 must refer to an enabled LED node"
#endif

/* ボード定義からGPIOデバイス、ピン番号、極性フラグを取り出す。 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

int main(void)
{
	int ret;
	bool ready = gpio_is_ready_dt(&led);

	printk("Stage 2: LED control\n");
	printk("GPIO device=%s, pin=%u, dt_flags=0x%x\n",
	       led.port->name, (unsigned int)led.pin, (unsigned int)led.dt_flags);
	printk("gpio_is_ready_dt: %s\n", ready ? "true" : "false");
	if (!ready) {
		printk("ERROR: GPIO device is not ready\n");
		return -ENODEV;
	}

	/* 極性を反映して、出力を最初は非アクティブ（消灯）にする。 */
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	printk("gpio_pin_configure_dt: %d\n", ret);
	if (ret < 0) {
		return ret;
	}

	/* GPIO_ACTIVE_LOWの反転はAPIに任せ、アプリは論理値を渡す。 */
	ret = gpio_pin_set_dt(&led, LED_STATE);
	printk("gpio_pin_set_dt: %d\n", ret);
	if (ret < 0) {
		return ret;
	}

	/* この表示は期待動作。実際の点灯／消灯は基板を見て確認する。 */
	printk("logical=%d, expected red LED=%s (check the board)\n",
	       LED_STATE, LED_STATE ? "ON" : "OFF");

	/* main終了後もGPIO出力は保持される。周期的な点滅は段階3で扱う。 */
	return 0;
}
