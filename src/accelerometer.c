/*
 * Copyright (c) 2018, Yannis Damigos
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "../include/accelerometer.h"


static int32_t readAccelerometer(const struct device *sensor,
			   enum sensor_channel channel)
{
	struct sensor_value val[3];
	int32_t ret = 0;

	ret = sensor_sample_fetch(sensor);
	if (ret < 0 && ret != -EBADMSG) {
		printf("Sensor sample update error\n");
		goto end;
	}

	ret = sensor_channel_get(sensor, channel, val);
	if (ret < 0) {
		printf("Cannot read sensor channels\n");
		goto end;
	}

	printf("( x y z ) = ( %f  %f  %f )\n", (sensor_value_to_double(&val[0])*57.3),
					       (sensor_value_to_double(&val[1])*57.3),
					       (sensor_value_to_double(&val[2])*57.3));

end:
	return ret;
}

void initAccelerometer(void)
{
	const struct device *const accelerometer = DEVICE_DT_GET_ONE(st_lis2dh);
	const struct device *const magnetometer = DEVICE_DT_GET_ONE(st_lis2mdl);

	if (!device_is_ready(accelerometer)) {
		printf("Device %s is not ready\n", accelerometer->name);
		return;
	}

	if (!device_is_ready(magnetometer)) {
		printf("Device %s is not ready\n", magnetometer->name);
		return;
	}

	while (1) {
		printf("Magnetometer data:\n");
		if (readAccelerometer(magnetometer, SENSOR_CHAN_MAGN_XYZ) < 0) {
			printf("Failed to read magnetometer data\n");
		}

		// printf("Accelerometer data:\n");
		// if (readAccelerometer(accelerometer, SENSOR_CHAN_ACCEL_XYZ) < 0) {
		// 	printf("Failed to read accelerometer data\n");
		// }

		k_sleep(K_MSEC(500));
	}
	// return 0;
}
int main(){
	initAccelerometer();
	return 0;
}
