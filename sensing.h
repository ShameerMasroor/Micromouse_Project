#ifndef SENSING_H
#define SENSING_H

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    float Ultrasonic_left;
    float Ultrasonic_front;
    float IR_front;
    float IR_left;
} sensors_t;

void initSensors(sensors_t sensor);
void readSensors();
bool isWallLeft();
bool isWallFront();
bool isWallRight();

#endif // SENSING_H
