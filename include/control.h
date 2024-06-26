#ifndef CONTROL_H
#define CONTROL_H
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <stdio.h>
#include "../include/algo.h"
#include "../include/sensing.h"
#include "motor.h"
#include "uart.h"
#include "sensing.h"

//#include "gyro.h"
//#include "accelerometer.h"

void control_thread();
void sensor_thread();
void communication_thread();
void motor_thread();
void algo_thread();

#endif // CONTROL_H
