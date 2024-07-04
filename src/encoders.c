#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include "../include/encoders.h"
#include <stdio.h>

#define LEFT_ENC DT_ALIAS(enc_left)
#define RIGHT_ENC DT_ALIAS(enc_right)

#if !DT_NODE_HAS_STATUS(LEFT_ENC, okay)
#error "Unsupported board: IR 1 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(RIGHT_ENC, okay)
#error "Unsupported board: IR 2 devicetree alias is not defined"
#endif

static const struct gpio_dt_spec left_enc_pin = GPIO_DT_SPEC_GET(LEFT_ENC, gpios);
static const struct gpio_dt_spec right_enc_pin = GPIO_DT_SPEC_GET(RIGHT_ENC, gpios);

encoders_t encoders = {.left_encoder_count = 0, .right_encoder_count = 0, .left_encoder_pin = left_enc_pin,
                    .right_encoder_pin = right_enc_pin};

// Callback function for left encoder
void left_encoder_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    encoders.left_encoder_count++;
    printk("Left encoder count: %d\n", encoders.left_encoder_count);
}

// Callback function for right encoder
void right_encoder_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    encoders.right_encoder_count++;
    printk("Right encoder count: %d\n", encoders.right_encoder_count);
}

static struct gpio_callback left_enc_cb_data;
static struct gpio_callback right_enc_cb_data;

void init_encoders()
{
    int ret;

    ret = gpio_pin_interrupt_configure_dt(&encoders.left_encoder_pin, GPIO_INT_EDGE_RISING);
    if (ret < 0) {
        printk("Error %d: failed to configure interrupt on left encoder pin\n", ret);
        return;
    }

    ret = gpio_pin_interrupt_configure_dt(&encoders.right_encoder_pin, GPIO_INT_EDGE_RISING);
    if (ret < 0) {
        printk("Error %d: failed to configure interrupt on right encoder pin\n", ret);
        return;
    }

    gpio_init_callback(&left_enc_cb_data, left_encoder_callback, BIT(encoders.left_encoder_pin.pin));
    gpio_add_callback(encoders.left_encoder_pin.port, &left_enc_cb_data);

    gpio_init_callback(&right_enc_cb_data, right_encoder_callback, BIT(encoders.right_encoder_pin.pin));
    gpio_add_callback(encoders.right_encoder_pin.port, &right_enc_cb_data);

    printk("Encoders initialized\n");
}

const double kp=0.00031;
const double kd=0;
double control_signal_left=0;
double control_signal_right=0;
static int error=0;  // the number of counts is always an integer number
static int difference=0;
static int last_error=0;
double base_pwm_r = 0.39;
double base_pwm_l = 0.4;
//const double base_pwm= 0.5;

double speed_matcher_right()
{
    error = (encoders.left_encoder_count - encoders.right_encoder_count); // error between the two motors
    // printf("The error is %d \n", error);
    difference = error - last_error;
    // printf("The difference is %d \n", difference);
    control_signal_right = base_pwm_r + (kp * error + kd * difference);  // PD controller

    // printf("Control PWM = %lf \n", control_signal);
    last_error = error;
    return control_signal_right;
}


double speed_matcher_left()
{
    error = (encoders.left_encoder_count - encoders.right_encoder_count); // error between the two motors
    // printf("The error is %d \n", error);
    difference = error - last_error;
    // printf("The difference is %d \n", difference);
    control_signal_left = base_pwm_l - (kp * error + kd * difference);  // PD controller

    // printf("Control PWM = %lf \n", control_signal);
    last_error = error;
    return control_signal_left;
}