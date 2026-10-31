/* SPDX-License-Identifier: Apache-2.0 */
#include <errno.h>
#include <stdbool.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>

#include "led_control.h"

#define LED_NODE DT_ALIAS(led0)
#if !DT_NODE_HAS_STATUS(LED_NODE, okay)
#error "led0 must refer to an enabled LED node"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
struct led_request {
	enum led_mode mode;
	uint32_t interval_ms;
};

K_MSGQ_DEFINE(led_requests, sizeof(struct led_request), 1, 4);
K_SEM_DEFINE(led_done, 0, 1);
K_THREAD_STACK_DEFINE(led_stack, 2048);
static struct k_thread led_thread;
static int request_result;
static bool initialized;
static atomic_t blink_error;

/* 初期設定後のGPIO更新と点滅状態は、このスレッドだけが担当する。 */
static void led_entry(void *p1, void *p2, void *p3)
{
	struct led_request request;
	enum led_mode mode = LED_OFF;
	uint32_t interval_ms = LED_BLINK_DEFAULT_MS;
	int logical = 0;

	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);
	while (1) {
		int ret = k_msgq_get(&led_requests, &request,
			mode == LED_BLINK ? K_MSEC(interval_ms) : K_FOREVER);

		if (ret == 0) {
			int next = request.mode != LED_OFF;

			request_result = gpio_pin_set_dt(&led, next);
			if (request_result == 0) {
				logical = next;
				mode = request.mode;
				interval_ms = request.interval_ms;
			} else {
				mode = LED_OFF; /* 自動切り替えを停止。実際のLED状態は不明。 */
			}
			k_sem_give(&led_done);
		} else if (ret == -EAGAIN && mode == LED_BLINK) {
			ret = gpio_pin_set_dt(&led, !logical);
			if (ret == 0) {
				logical = !logical;
			} else {
				atomic_set(&blink_error, ret);
				mode = LED_OFF;
			}
		}
	}
}

int led_control_init(void)
{
	int ret;

	if (!gpio_is_ready_dt(&led)) {
		return -ENODEV;
	}
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return ret;
	}
	k_thread_create(&led_thread, led_stack, K_THREAD_STACK_SIZEOF(led_stack),
		led_entry, NULL, NULL, NULL, 5, 0, K_NO_WAIT);
	initialized = true;
	return 0;
}

int led_control_apply(enum led_mode mode, uint32_t interval_ms)
{
	struct led_request request = {mode, interval_ms};
	int ret;

	if (!initialized) {
		return -ENODEV;
	}
	if ((mode != LED_OFF && mode != LED_ON && mode != LED_BLINK) ||
	    (mode == LED_BLINK && (interval_ms < LED_BLINK_MIN_MS ||
				  interval_ms > LED_BLINK_MAX_MS))) {
		return -EINVAL;
	}
	ret = k_msgq_put(&led_requests, &request, K_NO_WAIT);
	if (ret != 0) {
		return ret;
	}
	k_sem_take(&led_done, K_FOREVER);
	return request_result;
}

int led_control_take_error(void)
{
	return (int)atomic_clear(&blink_error);
}
