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

static sensor_data_t sensor_data = { .ir_data = NULL };
static char command;





void control_thread(void)
{   
    while (1)
    {
        /* get a data item */
        k_msgq_get(&sensing_control_q, &sensor_data, K_FOREVER);

        command = right_hand_follower(&sensor_data);
        // command = 'f';

        while (k_msgq_put(&motor_control_q, &command, K_NO_WAIT) != 0)
        {
            /* message queue is full: purge old data & try again */
            k_msgq_purge(&motor_control_q);
        }

        k_sleep(K_MSEC(1));
    }      
}

int main(void)
{
    printk("Micromouse Robot Starting...\n");
    initSensors();
    k_timer_start(&read_sensor_timer, K_SECONDS(0), K_SECONDS(3));
    return 0;
}

K_THREAD_DEFINE(control_thread_id, STACK_SIZE, control_thread, NULL, NULL, NULL, 10, 0, 0);