#include "../include/ultrasonic.h"
#include "../include/shared_mutex.h"  // Include the shared header file

void init_ultrasonic(const struct ultrasonic *sensor) {
    int ret;

    // Configure the trigger pin as an output
    ret = gpio_pin_configure_dt(&sensor->trig_spec, GPIO_OUTPUT);
    
    if (ret < 0) {
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure trigger pin\n", ret);
        k_mutex_unlock(&uart_mutex);
        return;
    }

    // Configure the echo pin as an input
    ret = gpio_pin_configure_dt(&sensor->echo_spec, GPIO_INPUT);
    if (ret < 0) {
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure echo pin\n", ret);
        
        return;
    }

    printk("Ultrasonic sensor initialized\n");
    k_mutex_unlock(&uart_mutex);
}


uint32_t measure_distance(const struct ultrasonic *sensor) {
    // This function assumes a busy-wait implementation.

    
    uint32_t start, end;
    bool echo_state=0;

    // Trigger the sensor by setting the trigger pin high for 10 microseconds
    gpio_pin_set(sensor->trig_spec.port, sensor->trig_spec.pin, 1);
    k_busy_wait(10); //blocking use sleep
    gpio_pin_set(sensor->trig_spec.port, sensor->trig_spec.pin, 0);
// k_busy_wait(1000); 
    // Wait for the echo pin to go high
    start = k_cycle_get_32();

    
    while(echo_state == 0){
        echo_state = gpio_pin_get(sensor->echo_spec.port, sensor->echo_spec.pin);
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Echo state is: %d\n", echo_state);
        k_mutex_unlock(&uart_mutex);
    }


    
    
    end = k_cycle_get_32();
    k_mutex_lock(&uart_mutex, K_FOREVER);
    printk("Echo state is now: %d\n", echo_state);
    
    // Calculate the pulse duration in microseconds
    uint32_t pulse_duration= k_cyc_to_us_floor32(end - start);

    // Calculate the distance in centimeters
    uint32_t distance_cm = 33129 * (pulse_duration/2000); 

    printk("Distance measured: %d cm\n", distance_cm);
    k_mutex_unlock(&uart_mutex);
    return distance_cm;
}
