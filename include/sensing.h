#ifndef SENSING_H
#define SENSING_H

#include <stdint.h>
#include <stdbool.h>
#include "IR.h"

typedef struct{
    ir *IR_Left;
    ir *IR_Front;
} sensors_t;

void initSensors(sensors_t *sensor, struct gpio_dt_spec left_pin, struct gpio_dt_spec front_pin);
void readSensors();
bool isWallLeft(sensors_t *sensor);
bool isWallFront(sensors_t *sensor);

#endif // SENSING_H
