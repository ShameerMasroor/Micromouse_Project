#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

// Ensure that your sensor node is defined in the device tree


const struct ultrasonic {
    
    struct gpio_dt_spec trig_spec;
    struct gpio_dt_spec echo_spec;
};

// static const struct ultrasonic ultrasonic_sensor = {
//     .trig_spec = GPIO_DT_SPEC_GET_OR(ULTRASONIC_TRIG_NODE, gpios, {0}),
//     .echo_spec = GPIO_DT_SPEC_GET_OR(ULTRASONIC_ECHO_NODE, gpios, {0}),
//     .num = 0,
// };

void init_ultrasonic(const struct ultrasonic *sensor);
uint32_t measure_distance(const struct ultrasonic *sensor);
