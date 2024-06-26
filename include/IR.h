#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>
#include <stdio.h>


// Ensure that your sensor node is defined in the device tree

const typedef struct{
   
   struct gpio_dt_spec d_out;
   char dir;
} ir;

void init_IR(const ir *IR);
bool wall_detected(const ir *IR);