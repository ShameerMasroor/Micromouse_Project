// /*
//  * Copyright (c) 2016 Intel Corporation
//  *
//  * SPDX-License-Identifier: Apache-2.0
//  */

// #include <stdio.h>
// #include <zephyr/kernel.h>
// #include <zephyr/drivers/gpio.h>

// /* 1000 msec = 1 sec */
// #define SLEEP_TIME_MS   1000

// /* The devicetree node identifier for the "led0" alias. */
// #define LED0_NODE DT_ALIAS(led3)
// // #define LED2_NODE DT_ALIAS(&led2)

// /*
//  * A build error on this line means your boardprintf("Hello World! %s\n", CONFIG_BOARD_TARGET);
//  is unsupported.
//  * See the sample documentation for information on how to fix this.
//  */
// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
// // static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED2_NODE, gpios);

// int main(void)
// {
// 	int ret;
// 	bool led_state = true;

// 	if (!gpio_is_ready_dt(&led)) {
// 		return 0;
// 	}

// 	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
// 	if (ret < 0) {
// 		return 0;
// 	}

// 	while (1) {
// 		ret = gpio_pin_toggle_dt(&led);
// 		if (ret < 0) {
// 			return 0;
// 		}

// 		led_state = !led_state;
// 		printf("LED state: %s\n", led_state ? "ON" : "OFF");
// 		k_msleep(SLEEP_TIME_MS);
// 	}

// 	//for testing the UART STLINK system on the board. By default, Zephyr uses UART 0
// 	printf("LED Blinking %s\n", CONFIG_BOARD_TARGET);

// 	return 0;
// }


/* 
Now we shall try to control the behavior of the LEDs
via threads. Good luck to us!
 
*/

//Thread implementation code

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

/* size of stack area used by each thread */
#define STACKSIZE 2048

/* scheduling priority used by each thread */
#define PRIORITY 7

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(LED1_NODE, okay)
#error "Unsupported board: led1 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(LED2_NODE, okay)
#error "Unsupported board: led1 devicetree alias is not defined"
#endif



struct led {
	struct gpio_dt_spec spec;
	uint8_t num;
};

static const struct led led0 = {
	.spec = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, {0}),
	.num = 0,
};

static const struct led led1 = {
	.spec = GPIO_DT_SPEC_GET_OR(LED1_NODE, gpios, {0}),
	.num = 1,
};

static const struct led led2 = {
	.spec = GPIO_DT_SPEC_GET_OR(LED2_NODE, gpios, {0}),
	.num = 2,
};

void blink(const struct led *led, uint32_t sleep_ms, uint32_t id)
{
	const struct gpio_dt_spec *spec = &led->spec;
	int cnt = 0;
	int ret;

	

	ret = gpio_pin_configure_dt(spec, GPIO_OUTPUT);
	

	while (1) {
		gpio_pin_set(spec->port, spec->pin, cnt % 2); //0 when 2 doesnt divide into cnt, 1 when 2 is a divisor

		k_msleep(sleep_ms);
		cnt++;
	}
}

void blink0(void)     //for led0
{
	blink(&led0, 100, 0);
}

void blink1(void)    //for led1
{
	blink(&led1, 1000, 1);
}

void blink2(void)    //for led1
{
	blink(&led2, 500, 2);

}

// void blink_runner(void)
// {
// blink(&led0, 100, 0);
// blink(&led1, 1000, 1);
// blink(&led2, 500, 2);
// }

// void master(void)
// {
K_THREAD_DEFINE(blink0_id, STACKSIZE, blink0, NULL, NULL, NULL,
		PRIORITY, 0, 0);
K_THREAD_DEFINE(blink1_id, STACKSIZE, blink1, NULL, NULL, NULL,
		PRIORITY, 0, 0);
K_THREAD_DEFINE(blink2_id, STACKSIZE, blink2, NULL, NULL, NULL,
		PRIORITY, 0, 0);
// }

// K_THREAD_DEFINE(master_thread, STACKSIZE, master, NULL, NULL, NULL,
// 		PRIORITY, 0, 0);