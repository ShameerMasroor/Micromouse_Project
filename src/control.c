#include "../include/control.h"

#define STACK_SIZE 2048
#define PRIORITY 7
K_MUTEX_DEFINE(uart_mutex);

//K_THREAD_DEFINE(control_thread_id, STACK_SIZE, control_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
//K_THREAD_DEFINE(communication_thread_id, STACK_SIZE, communication_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
//K_THREAD_DEFINE(motor_thread_id, STACK_SIZE, motor_thread, NULL, NULL, NULL, PRIORITY, 0, 0);

// Define Device Tree Aliases (or any other way to get the pins)
#define IR_OUT DT_ALIAS(dtout)
#define IR_OUT_2 DT_ALIAS(dtout2)

#define UART0_NODE DT_NODELABEL(usart1)  // as in our device tree

#if !DT_NODE_HAS_STATUS(IR_OUT, okay)
#error "Unsupported board: IR 1 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(IR_OUT_2, okay)
#error "Unsupported board: IR 2 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(UART0_NODE, okay)
#error "Unsupported board: uart0 devicetree alias is not defined"
#endif

static struct gpio_dt_spec left_pin = GPIO_DT_SPEC_GET_OR(IR_OUT, gpios, {0});
static struct gpio_dt_spec front_pin = GPIO_DT_SPEC_GET_OR(IR_OUT_2, gpios, {0});

// Instantiate the sensors_t struct
static sensors_t sensors = {
    .IR_Left = &(ir){ .d_out = {0}, .num = 1 },
    .IR_Front = &(ir){ .d_out = {0}, .num = 2 }
};

static uart_t uart = {
    .uart_dev = DEVICE_DT_GET(UART0_NODE)
};

//static motor_t motor;

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
    initSensors(&sensors,left_pin,front_pin);
    
    while (1) {
        //readSensors();
        // readGyroscope();
        // readAccelerometer();
        k_mutex_lock(&uart_mutex, K_FOREVER);
        detect.left = isWallLeft(&sensors);
        printk("Wall Status: %d cm\n", detect.left);
        detect.front = isWallFront(&sensors);
        printk("Wall Status: %d cm\n", detect.front);     
        k_mutex_unlock(&uart_mutex);
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

// void motor_thread() {
//     initMotorControl(&motor);
    
//     while (1) {
//         // Add motor control logic here
//         k_sleep(K_MSEC(1000));
//     }
// }

void algo_thread() {
    wallFollower(&maze, path, &detect);
    optimizePath(path, optimized_path);
    
    // while (1) {
    //     // If needed, add more algorithm logic here
    //     k_sleep(K_MSEC(1000));
    // }
}

int main(void) {
    initUart(&uart);
    printk("Micromouse Robot Starting...\n");
    init_motors();
    // // Start threads
    // k_thread_start(communication_thread_id);
    // k_thread_start(control_thread_id);
    // k_thread_start(motor_thread_id);
    
    
    // while (1) {
    //     // Main loop logic if needed
    //     k_sleep(K_MSEC(1000));
    // }

    return 0;
}

K_THREAD_DEFINE(algo_thread_id, STACK_SIZE, algo_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(sensor_thread_id, STACK_SIZE, sensor_thread, NULL, NULL, NULL, PRIORITY, 0, 0);

