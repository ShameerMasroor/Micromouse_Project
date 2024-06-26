#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct{
   
   struct gpio_dt_spec d_out;
   int num;
} ir;

void init_IR(ir *IR);
bool read_IR(ir *IR);