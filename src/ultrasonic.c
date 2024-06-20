#include "../include/ultrasonic.h"

void init_ultrasonic(const struct ultrasonic *sensor) {
    int ret;

    // Configure the trigger pin as an output
    ret = gpio_pin_configure_dt(&sensor->trig_spec, GPIO_OUTPUT);
    if (ret < 0) {
        printk("Error %d: failed to configure trigger pin\n", ret);
        return;
    }

    // Configure the echo pin as an input
    ret = gpio_pin_configure_dt(&sensor->echo_spec, GPIO_INPUT);
    if (ret < 0) {
        printk("Error %d: failed to configure echo pin\n", ret);
        return;
    }

    printk("Ultrasonic sensor initialized\n");
}

uint32_t measure_distance(const struct ultrasonic *sensor) {
    // This function assumes a busy-wait implementation.


    uint32_t start, end;
    int echo_state=0;

    // Trigger the sensor by setting the trigger pin high for 10 microseconds
    gpio_pin_set(sensor->trig_spec.port, sensor->trig_spec.pin, 1);
    k_busy_wait(10); //blocking use sleep
    gpio_pin_set(sensor->trig_spec.port, sensor->trig_spec.pin, 0);

    // Wait for the echo pin to go high
    start = k_cycle_get_32();

    
    while(echo_state == 0){
        echo_state = gpio_pin_get(sensor->echo_spec.port, sensor->echo_spec.pin);
        printk("Echo state is still 0 \n");
    }


    
    
    end = k_cycle_get_32();
    printk("Echo state is now 1 \n");

    // Calculate the pulse duration in microseconds
    uint32_t pulse_duration = k_cyc_to_us_floor32(end - start);

    // Calculate the distance in centimeters
    uint32_t distance_cm = 33129 * (pulse_duration/2000); 

    printk("Distance measured: %d cm\n", distance_cm);
    return distance_cm;
}
