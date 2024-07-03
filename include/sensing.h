#ifndef SENSING_H
#define SENSING_H

#include <stdint.h>
#include <stdbool.h>
#include "IR.h"

#define STACK_SIZE 2048
#define PRIORITY 7

extern struct k_msgq sensing_control_q;

typedef struct{
    ir *IR_Left;
    ir *IR_Front;
    
} sensors_t;

typedef struct{
    ir_data_t ir_data;
    //you may define an ultrasonic struct
} sensor_data_t;

void initSensors(void);
void readSensors(void);
void sensor_thread(void);

#endif // SENSING_H
