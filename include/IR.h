#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>
#include <stdio.h>

// extern int16_t adc_value;

typedef struct
{
   const struct gpio_dt_spec d_out;
} ir;

typedef struct
{
   bool left_ir_data;
   bool right_ir_data;
   bool front_ir_data;
   bool back_ir_data;
   int16_t right_analog_data;
} ir_data_t;

void init_IR(void);
ir_data_t read_IR(void);
static int read_adc(void);
static void configure_adc(void);