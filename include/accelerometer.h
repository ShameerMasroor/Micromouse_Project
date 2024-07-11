#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>
#include <zephyr/sys/util.h>
#include <math.h>
// typedef struct
// {
//     void;
// } accel_t;

// static int32_t initAccelerometer(const struct device *sensor,
// 			   enum sensor_channel channel);
// void readAccelerometer(accel_t accel);
void initIMU(void);
double readIMU();
double direction_controller_right();
double direction_controller_left();
double return_ref_yaw();
#endif // ACCELEROMETER_H
