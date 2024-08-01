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
    encoders.rotation_left_counter++;
    // printk("Left encoder count: %d\n", encoders.left_encoder_count);
}

// Callback function for right encoder
void right_encoder_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    encoders.right_encoder_count++;
    encoders.rotation_right_counter++;
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


static double control_signal_left = 0;
static double control_signal_right = 0;
static int error = 0;  // the number of counts is always an integer number
static double ir_error =0;
static double ir_error_sum=0;
// static int difference = 0;
static int last_error = 0;
static double error_sum = 0;

const double set_pointRPM = 90;

bool startup_flag = 1;


double clamp(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

double get_time_diff()
{
    static int64_t last_time = 0;
    int64_t current_time = k_uptime_get();
    double time_diff = (current_time - last_time) / 1000.0;  // convert milliseconds to seconds
    last_time = current_time;
    return time_diff;
}

const double KP = 0.005;
const double KI = 0.0005;
const double KD = 0.0;
const double DT = 0.05;
const double MAX_OUT = 0.75;
const double IR_SCALE_RIGHT_P = 0.000033;
// const double IR_SCALE_RIGHT_D = 0.00033;
const double IR_SCALE_LEFT_P = 0.000030;
// const double IR_SCALE_LEFT_D = 0.00033;
const double IR_SETPOINT = 300;
static double ir_difference=0;
static double last_error_ir=0;

const double scale_p_right = 0.67;
const double scale_i_right = 0.7;

const double scale_p_left = 0.65;
const double scale_i_left = 0.65;  

bool controller_state=1;

double speed_matcher_right()
{
    if (startup_flag==1){
        startup_flag=0;
        return 0.29;
    }

    if (controller_state){
    int16_t analog_ir_val = return_analog();
    int16_t front_analog_val = return_analog_front();
    

    

    int32_t  delta_slit;
    delta_slit = encoders.right_encoder_count; 
    float partial_rot = 0.0;
    partial_rot = (float)delta_slit/20.0f;
    double current_right_rpm = (double)(partial_rot*600.0f);
    double base_pwm_r = 0.3;
    double time_diff = 0.001;//get_time_diff();
    error = (set_pointRPM - current_right_rpm); // error between the two motors
    ir_error = (IR_SETPOINT - analog_ir_val);
    ir_error_sum +=ir_error;
    error_sum += error;
    ir_difference = ir_error-last_error_ir;
    last_error_ir = ir_error;

    
    // control_signal_right = base_pwm_r + (scale_p_right * KP * (error+IR_SCALE_RIGHT*ir_error) + scale_i_right * KI * (error_sum+IR_SCALE_RIGHT*ir_error_sum) * DT);  // PID controller
    control_signal_right = base_pwm_r + (scale_p_right * KP * (error) + scale_i_right * KI * (error_sum) * DT) + IR_SCALE_RIGHT_P*ir_error ;
    control_signal_right = clamp(control_signal_right, 0, MAX_OUT);  // Ensure control signal stays within [0, 1]
    
    // printf("\tControl signal for right: %lf current_right_rpm = %lf \n", control_signal_right, current_right_rpm);
    // printf("Right control signal: %lf\n", control_s600ignal_right);
        //printf("\tRight Motor Speed = %lf RPM \n", current_right_rpm);
    encoders.right_encoder_count =0;
    
    printk("Received right ADC value %d \n", analog_ir_val);
    printk("Received Front ADC value %d \n", front_analog_val);
    }
    return control_signal_right;
}




static double error_left;
// static double last_error_left;
static double time_diff_left;
static double error_sum_left;
static double control_signal_left;

double speed_matcher_left()
{   
    if (startup_flag==1){
        startup_flag=0;
        return 0.53;
    }

    if (controller_state){
    int16_t analog_ir_val = return_analog();
     
    double current_left_rpm = (double)(encoders.left_encoder_count/20.0f)*600.0f;

    double base_pwm_l = 0.3;
    time_diff_left = 0.001;//get_time_diff();
    error_left = (set_pointRPM - current_left_rpm); // error between the two motors
    error_sum_left += error_left;

    
    // control_signal_left = base_pwm_l + (scale_p_left * KP * (error-IR_SCALE_LEFT*ir_error) + scale_i_left * KI * (error_sum-IR_SCALE_LEFT*ir_error_sum) * DT) ;  // PID controller
    control_signal_left = base_pwm_l + (scale_p_left * KP * (error) + scale_i_left * KI * (error_sum) * DT) - IR_SCALE_LEFT_P*ir_error;
    control_signal_left = clamp(control_signal_left, 0, MAX_OUT);  // Ensure control signal stays within [0, 1]
    // printf("Control signal for left: %lf current_left_rpm = %lf \n", control_signal_left, current_left_rpm);

    // printf("left control signal: %lf\n", control_signal_left);
    
    encoders.left_encoder_count =0;
    //printf("Left Motor Speed = %lf RPM \n", current_left_rpm);
    }
    return control_signal_left;
}

double dia = 6.555;  // cm 
double revolutions = 0; 
// double circum = 3.14159 * dia;
int dist = 0;

int distance()
{
    revolutions = (encoders.rotation_left_counter + encoders.rotation_right_counter) * 1.03 / 40;
    dist = (int)(dia * 3.14159 * revolutions);
    // printf("Distance: %lf \n", dist);
    return dist;
}


void speed_detector(){ //to be called periodically at every one second
    double left_motor_speed = (encoders.left_encoder_count/20.0)*60.0;
    printf("Left counts = %d \n", encoders.left_encoder_count);
    double right_motor_speed = (encoders.right_encoder_count/20.0)*60.0;
    printf("Right counts = %d \n", encoders.right_encoder_count);
    // printf("Right Motor Speed = %lf RPM \n", right_motor_speed);
    encoders.left_encoder_count =0;
    encoders.right_encoder_count =0;

}

int return_left_enc_count(){
    return encoders.rotation_left_counter;
}


int return_right_enc_count(){
    return encoders.rotation_right_counter;
}

void set_controller(bool flag){
    controller_state = flag;
}