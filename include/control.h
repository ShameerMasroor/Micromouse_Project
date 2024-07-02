#ifndef CONTROL_H
#define CONTROL_H
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <stdio.h>
#include "algo.h"
#include "motor.h"
#include "uart.h"
#include "sensing.h"
#include "shared_mutex.h" 
#include "sensing_control_q.h"
#include <stdbool.h>

//#include "gyro.h"
//#include "accelerometer.h"
void control_thread(void);

#endif // CONTROL_H
