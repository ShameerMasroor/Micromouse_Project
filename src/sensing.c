#include "../include/sensing.h"
// #include "../include/sensing_control_q.h"

//static sensors_t sensor;
static sensor_data_t sensor_data;

K_MSGQ_DEFINE(sensing_control_q, sizeof(sensor_data_t), 10, 1);

void initSensors() 
{
    init_IR();
    init_encoders();
    printk("Sensors Initialized :) \n");
}

void readSensors() 
{
    sensor_data.ir_data = read_IR();
    sensor_data.encoder_data_left = speed_matcher_left();
    sensor_data.encoder_data_right = speed_matcher_right();
    // sensor_data.encoder_left_count = encoders.left_encoder_count;
    // sensor_data.encoder_right_count = encoders.right_encoder_count;

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
        printk("Put data into the sensor queue.\n");
        // k_sleep(K_MSEC(1));
        /* data item was successfully added to message queue */
    }
}

// K_THREAD_DEFINE(sensor_thread_id, STACK_SIZE, sensor_thread, NULL, NULL, NULL, PRIORITY, 0, 0);


void my_work_handler(struct k_work *work) {   //THE WORK OBJECT WILL HAVE THIS FUNCTIONALITY    
    // initSensors();
    
        readSensors();
        /*send data to consumers */
        while (k_msgq_put(&sensing_control_q, &sensor_data, K_NO_WAIT) != 0) {
            /* message queue is full: purge old data & try again */
            k_msgq_purge(&sensing_control_q);
            
        }
        printk("Data successfully into the sensor queue.\n");
        // k_sleep(K_MSEC(1));
        /* data item was successfully added to message queue */
    printk("Work handler called\n");
    // Add your custom processing here
}

K_WORK_DEFINE(read_sensor_object, my_work_handler);

void my_timer_handler(struct k_timer *dummy) {
    k_work_submit(&read_sensor_object);
}

K_TIMER_DEFINE(read_sensor_timer, my_timer_handler, NULL);





void my_speed_handler(struct k_work *work) {   //THE WORK OBJECT WILL HAVE THIS FUNCTIONALITY    
    // initSensors();
        speed_detector();
    
}

K_WORK_DEFINE(read_speed_object, my_speed_handler);

void my_speed_timer_handler(struct k_timer *dummy) {
    k_work_submit(&read_speed_object);
}

K_TIMER_DEFINE(read_speed_timer, my_speed_timer_handler, NULL);
//ADD LINE IN MAIN:
// k_timer_start(&read_sensor_timer, K_SECONDS(0), K_SECONDS(1));
