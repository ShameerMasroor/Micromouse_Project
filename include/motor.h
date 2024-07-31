#pragma once
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/pwm.h>
#include <stdio.h>
#include "accelerometer.h" 

extern struct k_msgq motor_control_q;

typedef struct {
    const struct gpio_dt_spec in1;
    const struct gpio_dt_spec in2;
    const struct gpio_dt_spec in3;
    const struct gpio_dt_spec in4;
    const struct  pwm_dt_spec enA;
    const struct  pwm_dt_spec enB;
} motors;

typedef struct
{
    char command;
    double pwm_data_left;
    double pwm_data_right;
    double yaw_controlled_pwm_right;
    double yaw_controlled_pwm_left;
    double yaw_angle;
    double ref_yaw;
    int left_enc_count;
    int right_enc_count;
    int16_t right_analog_ir;
    int distance;
    bool right_ir;
    bool front_ir;
    int front_ir_analog;
} feedback_t;

void init_motors(void);
void setMotorDirection(char direction);
void set_Speed(float m1_speed, float m2_speed, const motors *motors);
void motor_thread(void);
void turn_right();