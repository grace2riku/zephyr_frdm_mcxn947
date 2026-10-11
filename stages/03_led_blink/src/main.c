/* SPDX-License-Identifier: Apache-2.0 */

#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

/* 状態を変更した後の待機時間。点灯から次の点灯までは約2倍。 */
#define BLINK_INTERVAL_MS 250

#if BLINK_INTERVAL_MS <= 0
#error "BLINK_INTERVAL_MS must be positive"
#endif

#define LED_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(LED_NODE, okay)
#error "led0 must refer to an enabled LED node"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

int main(void)
{
	int ret;
	int logical = 1;
	bool ready = gpio_is_ready_dt(&led);

	printk("Stage 3: LED blink\n");
	printk("GPIO device=%s, pin=%u, dt_flags=0x%x\n",
	       led.port->name, (unsigned int)led.pin, (unsigned int)led.dt_flags);
	printk("gpio_is_ready_dt: %s\n", ready ? "true" : "false");
	if (!ready) {
		printk("ERROR: GPIO device is not ready\n");
		return -ENODEV;
	}

	/* 段階2と同じく、最初は極性を反映した消灯状態に設定する。 */
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	printk("gpio_pin_configure_dt: %d\n", ret);
	if (ret < 0) {
		return ret;
	}
	printk("sleep_ms=%d, nominal_period_ms=%d\n",
	       BLINK_INTERVAL_MS, 2 * BLINK_INTERVAL_MS);

	while (1) {
		/* 論理1は点灯、論理0は消灯。物理的な極性の反映はAPIに任せる。 */
		ret = gpio_pin_set_dt(&led, logical);
		if (ret < 0) {
			printk("ERROR: gpio_pin_set_dt: %d\n", ret);
			return ret;
		}

		/* GPIO設定直後の起動後経過時間。LEDの光や電圧の測定ではない。 */
		printk("uptime_ms=%lld, gpio_pin_set_dt=%d, logical=%d, "
		       "expected red LED=%s (check the board)\n",
		       (long long)k_uptime_get(), ret, logical, logical ? "ON" : "OFF");

		/* mainスレッドを待機させる。待機中もGPIOの出力は保持される。 */
		k_msleep(BLINK_INTERVAL_MS);
		/* 次の反復で渡す論理値を1→0→1と交互に変える。 */
		logical = !logical;
	}
}
