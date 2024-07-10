#ifndef GYRO_H
#define GYRO_H

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define SAMPLING_INTERVAL_MS 10
#define DISPLAY_INTERVAL_MS 50

// typedef struct
// {
//     void;
// } gyro_t;


// void initGyroscope(gyro_t* gyro);
// void readGyroscope(gyro_t gyro);

static struct sensor_value fetch_and_display_gyro(const struct device *sensor);
void initGyro(void);


#endif // GYRO_H
