/* SPDX-License-Identifier: Apache-2.0 */
#ifndef STAGE7_LED_CONTROL_H
#define STAGE7_LED_CONTROL_H

#include <stdint.h>

#define LED_BLINK_MIN_MS 100U
#define LED_BLINK_MAX_MS 2000U
#define LED_BLINK_DEFAULT_MS 500U

enum led_mode { LED_OFF, LED_ON, LED_BLINK };

int led_control_init(void);
/* main専用。LEDスレッドがGPIOへ反映した結果を待つ。 */
int led_control_apply(enum led_mode mode, uint32_t interval_ms);
int led_control_take_error(void);

#endif
