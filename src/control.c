#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/algo.h"
#include "../include/motor.h"
#include "../include/uart.h"
#include "../include/sensing.h"


#define STACK_SIZE 2048
#define PRIORITY 7

static sensor_data_t sensor_data;

static feedback_t feedback;

void control_thread(void)
{   
    while (1)
    {
        /* get a data item */
        k_msgq_get(&sensing_control_q, &sensor_data, K_FOREVER);

        // feedback.command = right_hand_follower(&sensor_data);
        feedback.pwm_data_left = sensor_data.encoder_data_left;
        feedback.pwm_data_right = sensor_data.encoder_data_right;
        feedback.yaw_controlled_pwm_right = sensor_data.yaw_controlled_pwm_right;
        feedback.yaw_controlled_pwm_left = sensor_data.yaw_controlled_pwm_left;
        feedback.yaw_angle = sensor_data.yaw_angle;
        feedback.ref_yaw = sensor_data.ref_yaw;
        feedback.left_enc_count = sensor_data.left_enc_count;
        feedback.right_enc_count = sensor_data.right_enc_count;
        feedback.right_analog_ir = sensor_data.ir_data.right_analog_data;
        feedback.distance = sensor_data.dist;
        feedback.right_ir = sensor_data.ir_data.right_ir_data;
        feedback.front_ir = sensor_data.ir_data.front_ir_data;
        feedback.front_ir_analog = sensor_data.ir_data.front_analog_data;
        feedback.command = 'f';
        while (k_msgq_put(&motor_control_q, &feedback, K_NO_WAIT) != 0)
        {
            /* message queue is full: purge old data & try again */
            k_msgq_purge(&motor_control_q);
        }

        // k_sleep(K_MSEC(1));
    }      
}

int main(void)
{
    printk("Micromouse Robot Starting...\n");
    initSensors();
    k_timer_start(&read_sensor_timer, K_SECONDS(0), K_MSEC(50)); //40 Hz
    // k_timer_start(&read_speed_timer, K_SECONDS(0), K_SECONDS(1));
    return 0;
}

K_THREAD_DEFINE(control_thread_id, STACK_SIZE, control_thread, NULL, NULL, NULL, 10, 0, 0);