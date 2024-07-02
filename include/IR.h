#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct{
   
   const struct gpio_dt_spec d_out;
} ir;


typedef struct{

   bool left_ir_data;
   bool right_ir_data;
   bool front_ir_data;
   bool back_ir_data;
} ir_data_t;


void init_IR(ir *IR_left, ir *IR_front);
ir_data_t read_IR(ir *IR_left, ir *IR_front);