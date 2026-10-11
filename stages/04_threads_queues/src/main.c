/* SPDX-License-Identifier: Apache-2.0 */

#include <errno.h>
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define LED_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(LED_NODE, okay)
#error "led0 must refer to an enabled LED node"
#endif

#define LED_STACK_SIZE 2048
#define COMMAND_STACK_SIZE 2048
#define LED_PRIORITY 5
#define COMMAND_PRIORITY 6
#define INITIAL_INTERVAL_MS 500
#define MIN_INTERVAL_MS 100
#define MAX_INTERVAL_MS 2000

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

/* 指示の種類。STOPはスレッド終了ではなく、LEDの点滅停止。 */
enum led_command_type {
	LED_CMD_START,
	LED_CMD_STOP,
	LED_CMD_SET_INTERVAL,
};

/* キューにはこの構造体の値をコピーする。ポインタを送らない。 */
struct led_command {
	enum led_command_type type;
	uint32_t interval_ms; /* ON/OFFの切り替え間隔。1周期は名目上この2倍。 */
	uint32_t sequence;    /* UARTで受信順を確認するための指示番号。 */
};

/* 未受信の指示を最大4件保存。最後の1はバッファのアラインメント。 */
K_MSGQ_DEFINE(led_msgq, sizeof(struct led_command), 4, 1);

K_THREAD_STACK_DEFINE(led_stack, LED_STACK_SIZE);
K_THREAD_STACK_DEFINE(command_stack, COMMAND_STACK_SIZE);
static struct k_thread led_thread;
static struct k_thread command_thread;

static const char *command_name(enum led_command_type type)
{
	switch (type) {
	case LED_CMD_START:
		return "START";
	case LED_CMD_STOP:
		return "STOP";
	case LED_CMD_SET_INTERVAL:
		return "SET_INTERVAL";
	default:
		return "UNKNOWN";
	}
}

/* 呼び出すのはLEDスレッドだけ。表示は要求した状態で、実測値ではない。 */
static int set_led(int logical, const char *reason)
{
	int ret = gpio_pin_set_dt(&led, logical);

	printk("LED uptime_ms=%lld, reason=%s, set_ret=%d, logical=%d, expected=%s\n",
	       (long long)k_uptime_get(), reason, ret, logical, logical ? "ON" : "OFF");
	return ret;
}

static void led_entry(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	/* LED状態はこのスレッドのローカル変数。他スレッドは直接変更しない。 */
	bool running = false;
	int logical = 0;
	uint32_t interval_ms = INITIAL_INTERVAL_MS;
	struct led_command command;
	int ret;
	bool ready = gpio_is_ready_dt(&led);

	printk("GPIO device=%s, pin=%u, dt_flags=0x%x, ready=%s\n",
	       led.port->name, (unsigned int)led.pin, (unsigned int)led.dt_flags,
	       ready ? "true" : "false");
	if (!ready) {
		printk("ERROR: GPIO device is not ready; LED thread exits\n");
		return;
	}
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	printk("gpio_pin_configure_dt=%d, initial expected=OFF\n", ret);
	if (ret < 0) {
		return;
	}

	while (1) {
		/* 停止中は指示まで待つ。点滅中は指示か切り替え時刻まで待つ。 */
		k_timeout_t timeout = running ? K_MSEC(interval_ms) : K_FOREVER;

		ret = k_msgq_get(&led_msgq, &command, timeout);
		if (ret == -EAGAIN && running) {
			int next = !logical;

			if (set_led(next, "TIMEOUT") < 0) {
				return;
			}
			logical = next;
			continue;
		}
		if (ret != 0) {
			printk("ERROR: k_msgq_get=%d; LED thread exits\n", ret);
			return;
		}

		printk("RX uptime_ms=%lld, seq=%u, cmd=%s, value_ms=%u\n",
		       (long long)k_uptime_get(), (unsigned int)command.sequence,
		       command_name(command.type), (unsigned int)command.interval_ms);
		switch (command.type) {
		case LED_CMD_START:
			if (!running) {
				if (set_led(1, "START") < 0) {
					return;
				}
				logical = 1;
				running = true;
			}
			break;
		case LED_CMD_STOP:
			if (set_led(0, "STOP") < 0) {
				return;
			}
			logical = 0;
			running = false;
			break;
		case LED_CMD_SET_INTERVAL:
			if (command.interval_ms < MIN_INTERVAL_MS ||
			    command.interval_ms > MAX_INTERVAL_MS) {
				printk("REJECT: interval must be %d..%d ms\n",
				       MIN_INTERVAL_MS, MAX_INTERVAL_MS);
				break;
			}
			interval_ms = command.interval_ms;
			break;
		default:
			printk("REJECT: unknown command\n");
			break;
		}
		printk("STATE running=%d, interval_ms=%u, nominal_period_ms=%u, logical=%d\n",
		       running, (unsigned int)interval_ms,
		       (unsigned int)(2U * interval_ms), logical);
	}
}

static void command_entry(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	/* 自動操作を繰り返す。UART入力は後の段階で学ぶ。 */
	static const struct {
		enum led_command_type type;
		uint32_t interval_ms;
		int32_t hold_ms;
	} script[] = {
		{LED_CMD_SET_INTERVAL, 500, 0},
		{LED_CMD_START, 0, 4000},
		{LED_CMD_START, 0, 0}, /* 点滅中に再度STARTしても終了しない。 */
		{LED_CMD_SET_INTERVAL, 250, 4000},
		{LED_CMD_STOP, 0, 2000},
		{LED_CMD_STOP, 0, 0},  /* 停止中のSTOPも受け付ける。 */
		{LED_CMD_START, 0, 4000},
		{LED_CMD_STOP, 0, 2000},
	};
	uint32_t sequence = 0;

	while (1) {
		for (size_t i = 0; i < ARRAY_SIZE(script); ++i) {
			struct led_command command = {
				.type = script[i].type,
				.interval_ms = script[i].interval_ms,
				.sequence = ++sequence,
			};
			/* 満杯ならCPUを譲って最大1秒待つ。失敗は無限再試行しない。 */
			int ret = k_msgq_put(&led_msgq, &command, K_MSEC(1000));

			if (ret != 0) {
				printk("ERROR: k_msgq_put=%d, seq=%u; command thread exits\n",
				       ret, (unsigned int)command.sequence);
				return;
			}
			if (script[i].hold_ms > 0) {
				k_msleep(script[i].hold_ms);
			}
		}
	}
}

int main(void)
{
	printk("Stage 4: threads and message queue\n");
	printk("LED priority=%d, requested_stack=%d B; command priority=%d, "
	       "requested_stack=%d B\n",
	       LED_PRIORITY, LED_STACK_SIZE, COMMAND_PRIORITY, COMMAND_STACK_SIZE);
	printk("msg_size=%u B, capacity=4; initial interval=%d ms\n",
	       (unsigned int)sizeof(struct led_command), INITIAL_INTERVAL_MS);

	/* K_FOREVERで作成時の実行を保留し、両方を準備してから開始する。 */
	k_thread_create(&led_thread, led_stack, K_THREAD_STACK_SIZEOF(led_stack),
			led_entry, NULL, NULL, NULL, LED_PRIORITY, 0, K_FOREVER);
	k_thread_create(&command_thread, command_stack, K_THREAD_STACK_SIZEOF(command_stack),
			command_entry, NULL, NULL, NULL, COMMAND_PRIORITY, 0, K_FOREVER);
	k_thread_start(&led_thread);
	k_thread_start(&command_thread);
	/* mainが終了しても、開始した2スレッドは実行を続ける。 */
	return 0;
}
