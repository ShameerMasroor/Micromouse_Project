#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include "uart.h"

typedef struct
{
    const struct gpio_dt_spec trig_spec;
    const struct gpio_dt_spec echo_spec;
} ultrasonic;

void init_ultrasonic(const struct ultrasonic *sensor);
uint32_t measure_distance(const struct ultrasonic *sensor);