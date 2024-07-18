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
// extern char command_dir;
// extern bool release_command=1;
void initIMU(void);
void set_direction(char dir);
double readIMU();
double direction_controller_right();
double direction_controller_left();
double return_ref_yaw();
void right_turn(void);
void left_turn(void);
int enable_returner();
bool return_go_forward();
void set_return_go_forward(bool setting);
#endif // ACCELEROMETER_H
