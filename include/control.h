#ifndef CONTROL_H
#define CONTROL_H

#include <zephyr.h>
#include "algo.h"
#include "motor.h"
#include "uart.h"
#include "sensing.h"
#include "gyro.h"
#include "accelerometer.h"

void control_thread();
void sensor_thread();
void communication_thread();
void motor_thread();
void algo_thread();

#endif // CONTROL_H
