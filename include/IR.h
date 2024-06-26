#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>
#include <stdio.h>

const typedef struct{
   
   struct gpio_dt_spec d_out;
   int num;
} ir;

void init_IR(const ir *IR);
bool wall_detected(const ir *IR);