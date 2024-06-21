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
    uint32_t start, end, pulse_duration;
    uint32_t cycles_per_sec = sys_clock_hw_cycles_per_sec();
    bool echo_state = false;
    int timeout = 1000000; // Timeout counter

    // Trigger the sensor by setting the trigger pin high for 10 microseconds
    gpio_pin_set(sensor->trig_spec.port, sensor->trig_spec.pin, 1);
    k_busy_wait(10); // 10 microseconds
    gpio_pin_set(sensor->trig_spec.port, sensor->trig_spec.pin, 0);

    // Wait for the echo pin to go high
    start = k_cycle_get_32();
    while (!echo_state && timeout-- > 0) {
        echo_state = gpio_pin_get(sensor->echo_spec.port, sensor->echo_spec.pin);
    }

    //the if condition will be entered when the while loop will be broken out of due to the timeout
    if (timeout <= 0) {
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Timeout occured waiting for echo pin to go high. Ping failed.\n");
        k_mutex_unlock(&uart_mutex);
        return 0;
    }

    // Wait for the echo pin to go low
    timeout = 1000000;
    while (echo_state && timeout-- > 0) {
        echo_state = gpio_pin_get(sensor->echo_spec.port, sensor->echo_spec.pin);
    }
    end = k_cycle_get_32();
    if (timeout <= 0) {
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Timeout waiting for echo pin to go low\n");
        k_mutex_unlock(&uart_mutex);
        return 0;
    }

    // Calculate the pulse duration in microseconds
    pulse_duration = (end - start) * 1e6 / cycles_per_sec;

    // Calculate the distance in centimeters
    uint32_t distance_cm = (pulse_duration / 58)-8;   //-12 due to the offset from the coils to the receiver plates

    // Print the pulse duration and distance measured
    k_mutex_lock(&uart_mutex, K_FOREVER);
    printk("Pulse duration: %d us\n", pulse_duration);
    printk("Distance measured: %d cm\n", distance_cm);
    k_mutex_unlock(&uart_mutex);

    return distance_cm;
}