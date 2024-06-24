#ifndef SENSING_H
#define SENSING_H

#include <stdint.h>
#include <stdbool.h>
#include "IR.h"

typedef struct{
    const struct ir *IR_Left;
    const struct ir *IR_Front;
} sensors_t;

void initSensors(sensors_t *sensor);
void readSensors();
bool isWallLeft(sensors_t *sensor);
bool isWallFront(sensors_t *sensor);

#endif // SENSING_H
