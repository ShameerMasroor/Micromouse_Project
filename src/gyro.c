#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define SAMPLING_INTERVAL_MS 10
#define DISPLAY_INTERVAL_MS 50

static int set_sampling_frequency(const struct device *sensor, double sampling_frequency)
{
    struct sensor_value setting;
    (void)sensor_value_from_double(&setting, sampling_frequency);
    return sensor_attr_set(sensor, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_SAMPLING_FREQUENCY, &setting);
}

struct sensor_value gyro[3];

// Function to calibrate gyro sensor and get bias values
static void calibrate_gyro(const struct device *sensor, double *bias)
{
    const int num_samples = 200;
    double sum_x = 0, sum_y = 0, sum_z = 0;

    for (int i = 0; i < num_samples; i++) {
        sensor_sample_fetch(sensor);
        sensor_channel_get(sensor, SENSOR_CHAN_GYRO_XYZ, gyro);
        sum_x += sensor_value_to_double(&gyro[0]);
        sum_y += sensor_value_to_double(&gyro[1]);
        sum_z += sensor_value_to_double(&gyro[2]);
        k_sleep(K_MSEC(SAMPLING_INTERVAL_MS));
    }

    bias[0] = sum_x / num_samples;
    bias[1] = sum_y / num_samples;
    bias[2] = sum_z / num_samples;
}


double integ_angle=0;
static double fetch_and_display(const struct device *sensor, double *orientation, double *bias, uint32_t *last_time)
{
    int rc = sensor_sample_fetch(sensor);

    if (rc != 0) {
        printf("ERROR: Failed fetching gyro values: %d.\n", rc);
        return 0;
    }

    rc = sensor_channel_get(sensor, SENSOR_CHAN_GYRO_XYZ, gyro);

    if (rc != 0) {
        printf("ERROR: Failed getting gyro values: %d\n", rc);
        return 0;
    }

    // Convert sensor values to double and subtract bias
    double gyro_x = sensor_value_to_double(&gyro[0]) - bias[0];
    double gyro_y = sensor_value_to_double(&gyro[1]) - bias[1];
    double gyro_z = sensor_value_to_double(&gyro[2]) - bias[2];

    // Get the actual elapsed time
    uint32_t current_time = k_uptime_get_32();
    double dt = (current_time - *last_time) / 1000.0; // Convert milliseconds to seconds
    *last_time = current_time;

    // Integrate to get orientation
    orientation[0] += gyro_x * dt;
    orientation[1] += gyro_y * dt;
    orientation[2] += gyro_z * dt;

    printf("%u ms: x %lf , y %lf , z %lf\n",
           current_time,
           orientation[0],
           orientation[1],
           orientation[2]);
	//GIVES Z AXIS DATA
	integ_angle= orientation[2];
	return integ_angle;
}


#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>
#include <zephyr/sys/util.h>
#include <math.h>
#define PI 3.141592654

double z_angle;
static double read_sensor(const struct device *sensor,
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

	printf("( x y z ) = ( %f  %f  %f )\n", sensor_value_to_double(&val[0]),
					       sensor_value_to_double(&val[1]),
					       sensor_value_to_double(&val[2]));
	z_angle = atan2f(sensor_value_to_double(&val[1]),sensor_value_to_double(&val[0]))*180.0/PI;
	printf("( about z angle ) = ( %f  )\n", atan2f(sensor_value_to_double(&val[1]),sensor_value_to_double(&val[0]))*180.0/PI);
end:
	return z_angle;
}



double new_angle=0 ;
double complementary_filter(double orientation, double accel_data)
{
    double alpha = 0.98; 

	
	new_angle += alpha* orientation + (1.0 - alpha) * accel_data;
	printf("%lf \n"), new_angle;
    return new_angle;

	


}


int main(void)
{
    const double sampling_frequency = 1000.0 / SAMPLING_INTERVAL_MS;
    const struct device *const sensor = DEVICE_DT_GET_ONE(st_i3g4250d);

    if (!device_is_ready(sensor)) {
        printf("Sensor %s is not ready\n", sensor->name);
        return 0;
    }

    printf("Set sensor sampling frequency to %f Hz.\n", sampling_frequency);
    set_sampling_frequency(sensor, sampling_frequency);

    double orientation[3] = {0, 0, 0}; // Initialize orientation to zero
    double bias[3] = {0, 0, 0};        // Initialize gyro bias to zero
    uint32_t last_time = k_uptime_get_32(); // Initialize the last time variable

    printf("Calibrating gyro...\n");
    calibrate_gyro(sensor, bias);
    printf("Gyro calibration complete. Bias: x %lf, y %lf, z %lf\n", bias[0], bias[1], bias[2]);

    printf("Start polling with an interval of %d ms\n", DISPLAY_INTERVAL_MS);


	// const struct device *const accelerometer = DEVICE_DT_GET_ONE(st_lis2dh);
	// const struct device *const magnetometer = DEVICE_DT_GET_ONE(st_lsm303dlhc_magn);

	// if (!device_is_ready(accelerometer)) {
	// 	printf("Device %s is not ready\n", accelerometer->name);
	// 	return 0;
	// }

	// if (!device_is_ready(magnetometer)) {
	// 	printf("Device %s is not ready\n", magnetometer->name);
	// 	return 0;
	// }


	double gyro=0;
	//double accel=0;
    while (true) {
        gyro = fetch_and_display(sensor, orientation, bias, &last_time);
        k_sleep(K_MSEC(DISPLAY_INTERVAL_MS));
		// printf("Accelerometer data:\n");

		// accel= read_sensor(accelerometer, SENSOR_CHAN_ACCEL_XYZ);
		// if (read_sensor(accelerometer, SENSOR_CHAN_ACCEL_XYZ) < 0) {
		// 	printf("Failed to read accelerometer data\n");


		// complementary_filter(gyro, accel);


    }
    
	

	// while (1) {
	// 	printf("Magnetometer data:\n");
	// 	if (read_sensor(magnetometer, SENSOR_CHAN_MAGN_XYZ) < 0) {
	// 		printf("Failed to read magnetometer data\n");
	// 	}

	// 	printf("Accelerometer data:\n");
	// 	if (read_sensor(accelerometer, SENSOR_CHAN_ACCEL_XYZ) < 0) {
	// 		printf("Failed to read accelerometer data\n");
	// 	}

		

		

	return 0;
	}