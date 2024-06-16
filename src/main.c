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

// #include <zephyr/kernel.h>
// #include <zephyr/device.h>
// #include <zephyr/drivers/gpio.h>
// #include <zephyr/sys/printk.h>
// #include <zephyr/sys/__assert.h>
// #include <string.h>

// /* size of stack area used by each thread */
// #define STACKSIZE 2048

// /* scheduling priority used by each thread */
// #define PRIORITY 7

// #define LED0_NODE DT_ALIAS(led0)
// #define LED1_NODE DT_ALIAS(led1)
// #define LED2_NODE DT_ALIAS(led2)

// #if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
// #error "Unsupported board: led0 devicetree alias is not defined"
// #endif

// #if !DT_NODE_HAS_STATUS(LED1_NODE, okay)
// #error "Unsupported board: led1 devicetree alias is not defined"
// #endif

// #if !DT_NODE_HAS_STATUS(LED2_NODE, okay)
// #error "Unsupported board: led1 devicetree alias is not defined"
// #endif



// struct led {
// 	struct gpio_dt_spec spec;
// 	uint8_t num;
// };

// static const struct led led0 = {
// 	.spec = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, {0}),
// 	.num = 0,
// };

// static const struct led led1 = {
// 	.spec = GPIO_DT_SPEC_GET_OR(LED1_NODE, gpios, {0}),
// 	.num = 1,
// };

// static const struct led led2 = {
// 	.spec = GPIO_DT_SPEC_GET_OR(LED2_NODE, gpios, {0}),
// 	.num = 2,
// };

// void blink(const struct led *led, uint32_t sleep_ms, uint32_t id)
// {
// 	const struct gpio_dt_spec *spec = &led->spec;
// 	int cnt = 0;
// 	int ret;

	

// 	ret = gpio_pin_configure_dt(spec, GPIO_OUTPUT);
	

// 	while (1) {
// 		gpio_pin_set(spec->port, spec->pin, cnt % 2); //0 when 2 doesnt divide into cnt, 1 when 2 is a divisor

// 		k_msleep(sleep_ms);
// 		cnt++;
// 	}
// }

// void blink0(void)     //for led0
// {
// 	blink(&led0, 100, 0);
// }

// void blink1(void)    //for led1
// {
// 	blink(&led1, 1000, 1);
// }

// void blink2(void)    //for led1
// {
// 	blink(&led2, 500, 2);

// }

// // void blink_runner(void)
// // {
// // blink(&led0, 100, 0);
// // blink(&led1, 1000, 1);
// // blink(&led2, 500, 2);
// // }

// // void master(void)
// // {
// K_THREAD_DEFINE(blink0_id, STACKSIZE, blink0, NULL, NULL, NULL,
// 		PRIORITY, 0, 0);
// K_THREAD_DEFINE(blink1_id, STACKSIZE, blink1, NULL, NULL, NULL,
// 		PRIORITY, 0, 0);
// K_THREAD_DEFINE(blink2_id, STACKSIZE, blink2, NULL, NULL, NULL,
// 		PRIORITY, 0, 0);
// // }

// K_THREAD_DEFINE(master_thread, STACKSIZE, master, NULL, NULL, NULL,
// 		PRIORITY, 0, 0);

//code for on-board button implementation
//----------------------------------------------------------------------------------------------
// #include <zephyr/kernel.h>
// #include <zephyr/device.h>
// #include <zephyr/drivers/gpio.h>
// #include <zephyr/sys/printk.h>
// #include <zephyr/sys/__assert.h>
// #include <string.h>
// #include <zephyr/drivers/uart.h> //be careful about the directory you choose!!!
// #include <stdio.h>

// /* size of stack area used by each thread */
// #define STACKSIZE 2048

// /* scheduling priority used by each thread */
// #define PRIORITY 7

// #define LED0_NODE DT_ALIAS(led0)
// #define BTN0_NODE DT_ALIAS(sw0)
// #define UART0_NODE DT_NODELABEL(usart1) 
// // #define UART1_NODE DT_ALIAS(uart1)


// #if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
// #error "Unsupported board: led0 devicetree alias is not defined"
// #endif

// #if !DT_NODE_HAS_STATUS(BTN0_NODE, okay)
// #error "Unsupported board: led1 devicetree alias is not defined"
// #endif


// #if !DT_NODE_HAS_STATUS(UART0_NODE, okay)
// #error "Unsupported board: uart0 devicetree alias is not defined"
// #endif


// struct led {
// 	struct gpio_dt_spec spec;
// 	uint8_t num;
// };

// struct button {
// 	struct gpio_dt_spec btn_spec;
// 	uint8_t btn_num;
// };

// static const struct led led0 = {
// 	.spec = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, {0}),
// 	.num = 0,
// };

// static const struct button btn0 = {
// 	.btn_spec = GPIO_DT_SPEC_GET_OR(BTN0_NODE, gpios, {0}),
// 	.btn_num = 0,
// };

// static const struct device *uart_dev = DEVICE_DT_GET(UART0_NODE);

// void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
// {
//     switch (evt->type) {
//     case UART_TX_DONE:
//         printk("UART_TX_DONE\n");
//         break;
//     case UART_RX_RDY:
//         printk("Received data: %s\n", evt->data.rx.buf);
//         break;
//     default:
//         break;
//     }
// }

// void blink(const struct button *btn, uint32_t btn_id, const struct led *led, uint32_t id)
// {
// 	const struct gpio_dt_spec *spec = &led->spec;
// 	const struct gpio_dt_spec *btn_spec = &btn->btn_spec;
	
// 	int ret_led;
// 	int ret_btn;

	

// 	ret_led = gpio_pin_configure_dt(spec, GPIO_OUTPUT);
// 	ret_btn = gpio_pin_configure_dt(btn_spec, GPIO_INPUT);

// 	while (1) {
//     int input_state = gpio_pin_get(btn_spec->port, btn_spec->pin); // Read input pin state

//     // Check if input is high (assuming high means active)
//     if (input_state) {
//         gpio_pin_set(spec->port, spec->pin, 1); // Turn on LED
// 		printk("\nLED is on\n");
//     } 

// 	else {
//         gpio_pin_set(spec->port, spec->pin, 0); // Turn off LED
// 		printk("\nLED is off\n");
//         printf("Hello World! %s\n", CONFIG_BOARD_TARGET);
//     }
// 	// k_msleep(100);
// }
// }

// void blink0(void)     //for led0
// {
// 	blink(&btn0, 0, &led0,0);
// }


// K_THREAD_DEFINE(blink0_id, STACKSIZE, blink0, NULL, NULL, NULL,
// 		PRIORITY, 0, 0);
// // K_THREAD_DEFINE(blink1_id, STACKSIZE, blink1, NULL, NULL, NULL,
// // 		PRIORITY, 0, 0);
// // K_THREAD_DEFINE(blink2_id, STACKSIZE, blink2, NULL, NULL, NULL,
// // 		PRIORITY, 0, 0);


// //testing UART capabilities
// //---------------------------------------------------------------------------------------------
// // 


//code for UART implementation

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>
#include <zephyr/drivers/uart.h>
#include <stdio.h>

/* size of stack area used by each thread */
#define STACKSIZE 2048

/* scheduling priority used by each thread */
#define PRIORITY 7

#define LED0_NODE DT_ALIAS(led0)
#define BTN0_NODE DT_ALIAS(sw0)
#define UART0_NODE DT_NODELABEL(usart1)  // as in our device tree

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(BTN0_NODE, okay)
#error "Unsupported board: btn0 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(UART0_NODE, okay)
#error "Unsupported board: uart0 devicetree alias is not defined"
#endif

struct led {
    struct gpio_dt_spec spec;
    uint8_t num;
};

struct button {
    struct gpio_dt_spec btn_spec;
    uint8_t btn_num;
};

static const struct led led0 = {
    .spec = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, {0}),
    .num = 0,
};

static const struct button btn0 = {
    .btn_spec = GPIO_DT_SPEC_GET_OR(BTN0_NODE, gpios, {0}),
    .btn_num = 0,
};

static const struct device *uart_dev = DEVICE_DT_GET(UART0_NODE);

void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
	if (!device_is_ready(uart_dev)) {
        printk("UART device not found!");
        return;
    }

    struct uart_config uart_cfg = {
        .baudrate = 115200,
        .parity = UART_CFG_PARITY_NONE,
        .stop_bits = UART_CFG_STOP_BITS_1,
        .data_bits = UART_CFG_DATA_BITS_8,
        .flow_ctrl = UART_CFG_FLOW_CTRL_NONE,
    };

    int err = uart_configure(uart_dev, &uart_cfg);
    if (err) {
        printk("Failed to configure UART device!");
        return;
    }

    printk("UART initialized.\n");
    switch (evt->type) {
    case UART_TX_DONE:
        printk("UART_TX_DONE\n");
        break;
    case UART_RX_RDY:
        printk("Received data: %s\n", evt->data.rx.buf);
        break;
    default:
        break;
    }
}

void blink(const struct button *btn, uint32_t btn_id, const struct led *led, uint32_t id)
{
    const struct gpio_dt_spec *spec = &led->spec;
    const struct gpio_dt_spec *btn_spec = &btn->btn_spec;

    int ret_led;
    int ret_btn;

    ret_led = gpio_pin_configure_dt(spec, GPIO_OUTPUT);
    ret_btn = gpio_pin_configure_dt(btn_spec, GPIO_INPUT);

    while (1) {
        int input_state = gpio_pin_get(btn_spec->port, btn_spec->pin); // Read input pin state

        // Check if input is high (assuming high means active)
        if (input_state) {
            gpio_pin_set(spec->port, spec->pin, 1); // Turn on LED
            printk("\nLED is on\n");
        } else {
            gpio_pin_set(spec->port, spec->pin, 0); // Turn off LED
            printk("\nLED is off\n"); //lowest overhead - best to use
            printf("Hello World!\n"); //contains formatting capabilities - highest overhead
            uart_poll_out(uart_dev, 'H');  // Send a character over UART, one character at a time - blocking in nature
        }
        // k_msleep(100);
    }
}

void blink0(void)     //for led0
{
    blink(&btn0, 0, &led0, 0);
}

void init_uart(void)  //you do not need this. You made the thread work using simply uart callback too
{
	uart_callback_set(uart_dev, uart_cb, NULL);
}




K_THREAD_DEFINE(uart_id, STACKSIZE, init_uart, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(blink0_id, STACKSIZE, blink0, NULL, NULL, NULL, PRIORITY, 0, 0);
