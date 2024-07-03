#include "../include/sensing.h"
// #include "../include/sensing_control_q.h"

static sensors_t sensor;
static sensor_data_t sensor_data = {.ir_data = NULL};

K_MSGQ_DEFINE(sensing_control_q, sizeof(sensor_data_t), 10, 1);

void initSensors() 
{
    init_IR(sensor.IR_Left, sensor.IR_Front);
    printk("Sensors Initialized :) \n");
}

void readSensors() 
{
    sensor_data.ir_data = read_IR(sensor.IR_Left, sensor.IR_Front);
}

void sensor_thread(void) //producer thread
{
    initSensors();
    while (1) {
        readSensors();
        /*send data to consumers */
        while (k_msgq_put(&sensing_control_q, &sensor_data, K_NO_WAIT) != 0) {
            /* message queue is full: purge old data & try again */
            k_msgq_purge(&sensing_control_q);
            
        }
        printk("Put data into the message queue.\n");
        k_sleep(K_SECONDS(1));
        /* data item was successfully added to message queue */
    }
}

K_THREAD_DEFINE(sensor_thread_id, STACK_SIZE, sensor_thread, NULL, NULL, NULL, PRIORITY, 0, 0);