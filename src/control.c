#include "../include/control.h"

#define STACK_SIZE 2048
#define PRIORITY 7

//K_THREAD_DEFINE(control_thread_id, STACK_SIZE, control_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(sensor_thread_id, STACK_SIZE, sensor_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
//K_THREAD_DEFINE(communication_thread_id, STACK_SIZE, communication_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
//K_THREAD_DEFINE(motor_thread_id, STACK_SIZE, motor_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(algo_thread_id, STACK_SIZE, algo_thread, NULL, NULL, NULL, PRIORITY, 0, 0);

K_MUTEX_DEFINE(uart_mutex);  //mutex definition

static uart_t uart;
static motor_t motor;
static sensors_t sensors;


static Detection detect;
static Maze maze;
static char path[100];
static char  optimized_path[100];

void control_thread() {
    while (1) {
        // Add control logic here
        k_sleep(K_MSEC(1000));
    }
}

void sensor_thread() {
    initSensors(&sensors);
    
    while (1) {
        //readSensors();
        // readGyroscope();
        // readAccelerometer();
        detect.left = isWallLeft(&sensors);
        detect.front = isWallFront(&sensors);
        // Process sensor data here
        // k_mutex_lock(&uart_mutex, K_FOREVER);
        // printk("Wall Status: %d cm\n  %d", dist, sensors.IR_Front);
        // k_mutex_unlock(&uart_mutex);
        k_sleep(K_MSEC(1000));

    }
}

void communication_thread() {
    initUart(&uart);
    
    while (1) {
        // Add UART communication handling here
        k_sleep(K_MSEC(1000));
    }
}

void motor_thread() {
    initMotorControl(&motor);
    
    while (1) {
        // Add motor control logic here
        k_sleep(K_MSEC(1000));
    }
}

void algo_thread() {
    wallFollower(&maze, path, &detect);
    optimizePath(path, optimized_path);
    
    while (1) {
        // If needed, add more algorithm logic here
        k_sleep(K_MSEC(1000));
    }
}

int main() {
    printk("Micromouse Robot Starting...\n");

    // // Start threads
    // k_thread_start(control_thread_id);
    k_thread_start(sensor_thread_id);
    // k_thread_start(communication_thread_id);
    // k_thread_start(motor_thread_id);
    k_thread_start(algo_thread_id);
    
    // while (1) {
    //     // Main loop logic if needed
    //     k_sleep(K_MSEC(1000));
    // }

    return 0;
}
