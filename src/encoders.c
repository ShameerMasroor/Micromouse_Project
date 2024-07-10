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
    // printk("Left encoder count: %d\n", encoders.left_encoder_count);
}

// Callback function for right encoder
void right_encoder_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    encoders.right_encoder_count++;
    // printk("Right encoder count: %d\n", encoders.right_encoder_count);
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

const double kp = 0.0025;
const double ki = 0.0001;
const double kd = 0.00002;
double control_signal_left = 0;
double control_signal_right = 0;
static int error = 0;  // the number of counts is always an integer number
static int difference = 0;
static int last_error = 0;
static double error_sum = 0;
double base_pwm_r = 0.2;
double base_pwm_l = 0.2;
static int64_t last_time = 0;

double clamp(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

double get_time_diff()
{
    int64_t current_time = k_uptime_get();
    double time_diff = (current_time - last_time) / 1000.0;  // convert milliseconds to seconds
    last_time = current_time;
    return time_diff;
}

double speed_matcher_right()
{
    double time_diff = get_time_diff();
    error = (encoders.left_encoder_count - encoders.right_encoder_count); // error between the two motors
    error_sum += error * time_diff;
    difference = (error - last_error) / time_diff;

    
    control_signal_right = base_pwm_r + (kp * error + ki * error_sum + kd * difference);  // PID controller
    control_signal_right = clamp(control_signal_right, 0.0, 0.5);  // Ensure control signal stays within [0, 1]
    

    // printk("Right control signal: %lf, Error: %d, Time diff: %lf\n", control_signal_right, error, time_diff);
    
    last_error = error;
    return control_signal_right;
}

double speed_matcher_left()
{
    double time_diff = get_time_diff();
    error = (encoders.left_encoder_count - encoders.right_encoder_count); // error between the two motors
    error_sum += error * time_diff;
    difference = (error - last_error) / time_diff;

    control_signal_left = base_pwm_l - (kp * error + ki * error_sum + kd * difference);  // PID controller
    control_signal_left = clamp(control_signal_left, 0.0, 1.0);  // Ensure control signal stays within [0, 1]

    printk("Error: %d\n", error);

    last_error = error;
    return control_signal_left;
}

double radius = 6.555;  // cm 
double revolutions = 0; 


double distance()
{
    revolutions = (encoders.left_encoder_count + encoders.right_encoder_count) * 3.14159;
    return (radius * revolutions);
}


void speed_detector(){ //to be called periodically at every one second
    double left_motor_speed = (encoders.left_encoder_count/20.0)*60.0;
    printf("Left counts = %d \n", encoders.left_encoder_count);
    printf("Left Motor Speed = %lf RPM \n", left_motor_speed);
    double right_motor_speed = (encoders.right_encoder_count/20.0)*60.0;
    printf("Right counts = %d \n", encoders.right_encoder_count);
    printf("Right Motor Speed = %lf RPM \n", right_motor_speed);
    encoders.left_encoder_count =0;
    encoders.right_encoder_count =0;

}


