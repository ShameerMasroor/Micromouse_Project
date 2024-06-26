#include "../include/motor.h"
#include "../include/shared_mutex.h"  // Include the shared header file
#include <zephyr/drivers/pwm.h>

void init_motors(const struct motors *motors){

int ret;

    // Configure the trigger pin as an output
    ret = gpio_pin_configure_dt(&motors->in1, GPIO_OUTPUT);
    
    if (ret < 0) {
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure in1 pin\n", ret);
        k_mutex_unlock(&uart_mutex);
        return;
    }

    // Configure the echo pin as an input
    ret = gpio_pin_configure_dt(&motors->in2, GPIO_OUTPUT);
    if (ret < 0) {
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure in2 pin\n", ret);
        
        return;
    }

    ret = gpio_pin_configure_dt(&motors->in3, GPIO_OUTPUT);
    if (ret < 0) {
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure in3 pin\n", ret);
        
        return;
    }

    ret = gpio_pin_configure_dt(&motors->in4, GPIO_OUTPUT);
    if (ret < 0) {
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure in4 pin\n", ret);
        
        return;
    }

    printk("Both motors initialized\n");
    k_mutex_unlock(&uart_mutex);



}
void setMotorDirection(const struct motors *motors,char direction){

    switch(direction){
        case 'f':  //forwards
            gpio_pin_set(motors->in1.port, motors->in1.pin, 1);
            gpio_pin_set(motors->in2.port, motors->in2.pin, 0);
            gpio_pin_set(motors->in3.port, motors->in3.pin, 1);
            gpio_pin_set(motors->in4.port, motors->in4.pin, 0);
            break;

        case 'b':  //backwards
            gpio_pin_set(motors->in1.port, motors->in1.pin, 0);
            gpio_pin_set(motors->in2.port, motors->in2.pin, 1);
            gpio_pin_set(motors->in3.port, motors->in3.pin, 0);
            gpio_pin_set(motors->in4.port, motors->in4.pin, 1);
            break;

        case 'r':  //right
            gpio_pin_set(motors->in1.port, motors->in1.pin, 1);
            gpio_pin_set(motors->in2.port, motors->in2.pin, 0);
            gpio_pin_set(motors->in3.port, motors->in3.pin, 0);
            gpio_pin_set(motors->in4.port, motors->in4.pin, 1);
            break;

        case 'l':  //left
            gpio_pin_set(motors->in1.port, motors->in1.pin, 0);
            gpio_pin_set(motors->in2.port, motors->in2.pin, 1);
            gpio_pin_set(motors->in3.port, motors->in3.pin, 1);
            gpio_pin_set(motors->in4.port, motors->in4.pin, 0);
            break;

        case 'h':  //halt
            gpio_pin_set(motors->in1.port, motors->in1.pin, 0);
            gpio_pin_set(motors->in2.port, motors->in2.pin, 0);
            gpio_pin_set(motors->in3.port, motors->in3.pin, 0);
            gpio_pin_set(motors->in4.port, motors->in4.pin, 0);
            break;

        default: //you may add break here in default in case the system doesnt perform correctly
            gpio_pin_set(motors->in1.port, motors->in1.pin, 0);
            gpio_pin_set(motors->in2.port, motors->in2.pin, 0);
            gpio_pin_set(motors->in3.port, motors->in3.pin, 0);
            gpio_pin_set(motors->in4.port, motors->in4.pin, 0);
    }
}


void set_Speed(uint32_t m1_speed, uint32_t m2_speed, const struct motors *motors){


    pwm_set_dt(&(motors->enA), 255, m1_speed);  //giving this a duty cycle
    // pwm_set_dt(motors->enB, 255, m2_speed);

    //

}