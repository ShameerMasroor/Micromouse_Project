#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>
#include <zephyr/sys/util.h>

// typedef struct
// {
//     void;
// } accel_t;

// static int32_t initAccelerometer(const struct device *sensor,
// 			   enum sensor_channel channel);
// void readAccelerometer(accel_t accel);
void initAccelerometer(void);
static int32_t readAccelerometer(const struct device *sensor, enum sensor_channel channel);

#endif // ACCELEROMETER_H
