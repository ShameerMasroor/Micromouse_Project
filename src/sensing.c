#include "sensing.h"

void initSensors(sensors_t *sensor) 
{
    init_IR(sensor->IR_Left);
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
