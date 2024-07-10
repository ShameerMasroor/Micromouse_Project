#ifndef SENSING_H
#define SENSING_H

#include <stdint.h>
#include <stdbool.h>
#include "IR.h"
#include "encoders.h"

#define STACK_SIZE 2048
#define PRIORITY 7

extern struct k_msgq sensing_control_q;
extern struct k_timer read_sensor_timer;


typedef struct{
    ir_data_t ir_data;
    double encoder_data_left;
    double encoder_data_right;
 
} sensor_data_t;

void initSensors(void);
void readSensors(void);
void sensor_thread(void);

#endif // SENSING_H
