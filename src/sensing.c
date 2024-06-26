#include "../include/sensing.h"

void initSensors(sensors_t *sensor, struct gpio_dt_spec left_pin, struct gpio_dt_spec front_pin) 
{
    // Initialize IR Left
    sensor->IR_Left->d_out = left_pin;
    init_IR(sensor->IR_Left);

    // Initialize IR Front
    sensor->IR_Front->d_out = front_pin;
    init_IR(sensor->IR_Front);
}

void readSensors() {
    // Dummy function to read sensor data
}

bool isWallLeft(sensors_t *sensor) {
    return wall_detected(sensor->IR_Left); // Return true if there's a wall on the left
}

bool isWallFront(sensors_t *sensor) {
    return wall_detected(sensor->IR_Front); // Return true if there's a wall on the front
}

bool isWallRight() {
    // Dummy function to check for right wall
    return false; // Return true if there's a wall on the right
}
