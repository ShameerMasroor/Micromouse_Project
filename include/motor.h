#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/pwm.h>
// #include "../include/shared_mutex.h"  

// Ensure that your sensor node is defined in the device tree


struct motors {
    
    const struct gpio_dt_spec in1;
    const struct gpio_dt_spec in2;
    const struct gpio_dt_spec in3;
    const struct gpio_dt_spec in4;
    const struct  pwm_dt_spec enA;
    const struct  pwm_dt_spec enB;

};

// static const struct ultrasonic ultrasonic_sensor = {
//     .trig_spec = GPIO_DT_SPEC_GET_OR(ULTRASONIC_TRIG_NODE, gpios, {0}),
//     .echo_spec = GPIO_DT_SPEC_GET_OR(ULTRASONIC_ECHO_NODE, gpios, {0}),
//     .num = 0,
// };

void init_motors(void);
void setMotorDirection(char direction);
void set_Speed(double m1_speed, double m2_speed, const struct motors *motors);

