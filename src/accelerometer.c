/*
 * Copyright (c) 2018, Yannis Damigos
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "../include/accelerometer.h"

double ref_yaw_angle;
double current_yaw_angle;
bool flag=0;

const struct device *const accelerometer = DEVICE_DT_GET_ONE(st_lis2dh);
const struct device *const magnetometer = DEVICE_DT_GET_ONE(st_lis2mdl);


double readIMU()
{
	const struct device *sensor_accel = accelerometer;
	enum sensor_channel channel_accel = SENSOR_CHAN_ACCEL_XYZ;
	const struct device *sensor_magno = magnetometer;
	enum sensor_channel channel_magno = SENSOR_CHAN_MAGN_XYZ;

	struct sensor_value accel_val[3];
	struct sensor_value magno_val[3];

	int32_t ret = 0;

	ret = sensor_sample_fetch(sensor_accel);
	if (ret < 0 && ret != -EBADMSG) {
		printf("Accelerometer sample update error_a\n");
		goto end;
	}

	ret = sensor_channel_get(sensor_accel, channel_accel, accel_val);
	if (ret < 0) {
		printf("Cannot read Accelerometer channels\n");
		goto end;
	}

	ret = sensor_sample_fetch(sensor_magno);
	if (ret < 0 && ret != -EBADMSG) {
		printf("Magnetometer sample update error_a\n");
		goto end;
	}

	ret = sensor_channel_get(sensor_magno, channel_magno, magno_val);
	if (ret < 0) {
		printf("Cannot read sensor channels\n");
		goto end;
	}

	double accelX = sensor_value_to_double(&accel_val[0]);
	double accelY = sensor_value_to_double(&accel_val[1]);
	double accelZ = sensor_value_to_double(&accel_val[2]);

	double pitch = atan2(accelY, sqrt((accelX * accelX)+(accelZ*accelZ)));
	double roll  = atan2(-accelX, sqrt((accelY * accelY)+(accelZ*accelZ)));

	double magX = sensor_value_to_double(&magno_val[0])*100;
	double magY = sensor_value_to_double(&magno_val[1])*100;
	double magZ = sensor_value_to_double(&magno_val[2])*100;

	double Yh = (magY * cos(roll)) - (magZ* sin(roll));
	double Xh = (magX*cos(pitch)) + (magY * sin(roll)*sin(pitch)) + (magZ*cos(roll)*sin(pitch));

	double yaw = atan2(Yh, Xh);

	roll = roll *57.3;
	pitch = pitch *57.3;
	yaw = yaw * 57.3;

	// if (yaw < 0){
	// 	yaw = 180 + yaw;
	// }

	//to store the reference yaw angle once
	if (flag==0){
		ref_yaw_angle = yaw;
		flag=1;
	}

	// if (abs(current_yaw_angle - yaw)>100){
	// 	return current_yaw_angle;
	// }

	current_yaw_angle = yaw;

	

	printf("Ref. Yaw angle: %lf \n", ref_yaw_angle);
	// printf("Pitch angle: %lf \n", pitch);
	printf("Current Yaw angle: %lf \n", yaw);
	// printf("( x y z ) = ( %f  %f  %f )\n", (sensor_value_to_double(&val[0])*57.3),
	// 				       (sensor_value_to_double(&val[1])*57.3),
	// 				       (sensor_value_to_double(&val[2])*57.3));
	
end:
	return current_yaw_angle;
}


void initIMU(void)
{
	
	if (!device_is_ready(accelerometer)) {
		printf("Device %s is not ready\n", accelerometer->name);
		return;
	}

	if (!device_is_ready(magnetometer)) {
		printf("Device %s is not ready\n", magnetometer->name);
		return;
	}
	// if (readIMU(accelerometer, SENSOR_CHAN_ACCEL_XYZ, magnetometer, SENSOR_CHAN_MAGN_XYZ) < 0) {
	// 		printf("Failed to read magnetometer data\n");
	// 	}
		// printf("Accelerometer data:\n");
		// if (readAccelerometer(accelerometer, SENSOR_CHAN_ACCEL_XYZ) < 0) {
		// 	printf("Failed to read accelerometer data\n");
		// }
		// k_sleep(K_MSEC(200));
	
	// return 0;
}


static double control_signal_left_accel = 0;
static double control_signal_right_accel = 0;
static int error_a = 0;  // the number of counts is always an integer number
static int difference_a = 0;
static int last_error_a = 0;
static double error_a_sum = 0;
static int64_t last_time_a = 0;

double clamp_accel(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

double get_time_diff_accel()
{
    int64_t current_time = k_uptime_get();
    double time_diff = (current_time - last_time_a) / 1000.0;  // convert milliseconds to seconds
    last_time_a = current_time;
    return time_diff;
}

const double kp_accel = 0.009;  //0.02 is a good value for a single right motor
const double ki_accel = 0.000;
const double kd_accel = 0.008; //0.008 is a good value for a single right motor

double direction_controller_right()
{
	
	double base_pwm_r = 0.55;
	// double base_pwm_l = 0.3;	
    double time_diff = get_time_diff_accel();
    error_a = (ref_yaw_angle - current_yaw_angle); // error_a between the two motors
    error_a_sum += error_a * time_diff;
    difference_a = (error_a - last_error_a) / time_diff;

    
    control_signal_right_accel = base_pwm_r + (kp_accel * error_a + ki_accel * error_a_sum + kd_accel * difference_a);  // PID controller
    control_signal_right_accel = clamp_accel(control_signal_right_accel, 0.4, 0.65);  // Ensure control signal stays within [0, 1]
    
    // printk("Right control signal: %lf, error_a: %d, Time diff: %lf\n", control_signal_right, error_a, time_diff);
    
    last_error_a = error_a;
	// printf("Motor Controlled PWM %lf \n", control_signal_right_accel);
    return control_signal_right_accel;
}

double direction_controller_left()
{
	
	double base_pwm_l = 0.45;
	// double base_pwm_l = 0.3;	
    double time_diff = get_time_diff_accel();
    error_a = (ref_yaw_angle - current_yaw_angle); // error_a between the two motors
    error_a_sum += error_a * time_diff;
    difference_a = (error_a - last_error_a) / time_diff;

    
    control_signal_left_accel = base_pwm_l - (kp_accel * error_a + ki_accel * error_a_sum + kd_accel * difference_a);  // PID controller
    control_signal_left_accel = clamp_accel(control_signal_right_accel, 0.4, 0.65);  // Ensure control signal stays within [0, 1]
    
    // printk("Right control signal: %lf, error_a: %d, Time diff: %lf\n", control_signal_right, error_a, time_diff);
    
    last_error_a = error_a;
	// printf("Motor Controlled PWM %lf \n", control_signal_right_accel);
	control_signal_left_accel=0.5;
    return control_signal_left_accel;
}

double return_ref_yaw(){
	return ref_yaw_angle;
}