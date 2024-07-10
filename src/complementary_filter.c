#include "../include/complementary_filter.h"
#include "gyro.c"
#include "accelerometer.c"


double complementary_angle(){
    struct sensor_value gyro[3];
    const double sampling_frequency = 1000.0 / SAMPLING_INTERVAL_MS;
	const struct device *const sensor = DEVICE_DT_GET_ONE(st_i3g4250d);

	if (!device_is_ready(sensor)) {
		printf("Sensor %s is not ready\n", sensor->name);
		return 0;
	}

	printf("Set sensor sampling frequency to %f Hz.\n", sampling_frequency);
	set_sampling_frequency(sensor, sampling_frequency);

	printf("Start polling with an interval of %d ms\n", DISPLAY_INTERVAL_MS);
	while (true) {
		gyro = fetch_and_display(sensor);

		/* Wait some time before printing the next value */
		k_sleep(K_MSEC(DISPLAY_INTERVAL_MS));
	}


    const struct device *const accelerometer = DEVICE_DT_GET_ONE(st_lis2dh);
	const struct device *const magnetometer = DEVICE_DT_GET_ONE(st_lsm303dlhc_magn);

	if (!device_is_ready(accelerometer)) {
		printf("Device %s is not ready\n", accelerometer->name);
		return 0;
	}

	if (!device_is_ready(magnetometer)) {
		printf("Device %s is not ready\n", magnetometer->name);
		return 0;
	}

	while (1) {
		printf("Magnetometer data:\n");
		if (read_sensor(magnetometer, SENSOR_CHAN_MAGN_XYZ) < 0) {
			printf("Failed to read magnetometer data\n");
		}

		printf("Accelerometer data:\n");
		if (read_sensor(accelerometer, SENSOR_CHAN_ACCEL_XYZ) < 0) {
			printf("Failed to read accelerometer data\n");
		}

		// k_sleep(K_MSEC(2000));
	}


    //
}