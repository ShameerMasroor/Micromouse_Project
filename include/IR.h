#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

// Ensure that your sensor node is defined in the device tree

const struct ir{
   
   struct gpio_dt_spec d_out;
   char dir;
};

void init_IR(const struct ir *IR);
bool wall_detected(const struct ir *IR);