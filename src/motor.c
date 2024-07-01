#include "../include/motor.h"
#include "../include/shared_mutex.h"  // Include the shared header file
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

#define IN1 DT_ALIAS(in1)
#define IN2 DT_ALIAS(in2)
#define IN3 DT_ALIAS(in3)
#define IN4 DT_ALIAS(in4)

static const struct gpio_dt_spec in1 = GPIO_DT_SPEC_GET(IN1, gpios);
static const struct gpio_dt_spec in2 = GPIO_DT_SPEC_GET(IN2, gpios);
static const struct gpio_dt_spec in3 = GPIO_DT_SPEC_GET(IN3, gpios);
static const struct gpio_dt_spec in4 = GPIO_DT_SPEC_GET(IN4, gpios);

static struct motors motor = {
    /*.in1 = GPIO_DT_SPEC_GET(IN1, gpios),
    .in2 = GPIO_DT_SPEC_GET(IN2, gpios),
    .in3 = GPIO_DT_SPEC_GET(IN3, gpios),
    .in4 = GPIO_DT_SPEC_GET(IN4, gpios),*/
    .enA = PWM_DT_SPEC_GET(DT_ALIAS(pwm_m1)),
    
    // .enA = pwm_motor1,
    .enB= PWM_DT_SPEC_GET(DT_ALIAS(pwm_m2)),
};


void init_motors(void){

int ret;

    // Configure the trigger pin as an output
    ret = gpio_pin_configure_dt(&in1, GPIO_OUTPUT);
    
    if (ret < 0) {
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure in1 pin\n", ret);
        k_mutex_unlock(&uart_mutex);
        return; 

    // Configure the echo pin as an input
    ret = gpio_pin_configure_dt(&in2, GPIO_OUTPUT);
    if (ret < 0) {
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure in2 pin\n", ret);
        k_mutex_unlock(&uart_mutex);
        return;
    }

    ret = gpio_pin_configure_dt(&in3, GPIO_OUTPUT);
    if (ret < 0) {
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure in3 pin\n", ret);
        k_mutex_unlock(&uart_mutex);
        return;
    }

    ret = gpio_pin_configure_dt(&in4, GPIO_OUTPUT);
    if (ret < 0) {
        k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure in4 pin\n", ret);
        k_mutex_unlock(&uart_mutex);
        return;
    }

	if (!pwm_is_ready_dt(&motor.enA)) {
		printk("Error: PWM device enA is not ready\n");
		return;
	}

	if (!pwm_is_ready_dt(&motor.enB)) {
		printk("Error: PWM device enB is not ready\n");
		return;
	}

    k_mutex_lock(&uart_mutex, K_FOREVER);
    printk("Both motors initialized\n");
    k_mutex_unlock(&uart_mutex);



}
}

void setMotorDirection(char direction){

    switch(direction){
        case 'f':  //forwards
            gpio_pin_set_dt(&in1, 1);
            gpio_pin_set_dt(&in2, 0);
            gpio_pin_set_dt(&in3, 1);
            gpio_pin_set_dt(&in4, 0);
            break;

        case 'b':  //backwards
            gpio_pin_set_dt(&in1, 0);
            gpio_pin_set_dt(&in2, 1);
            gpio_pin_set_dt(&in3, 1);
            gpio_pin_set_dt(&in4, 1);
            break;

        case 'r':  //right
            gpio_pin_set_dt(&in1, 1);
            gpio_pin_set_dt(&in2, 0);
            gpio_pin_set_dt(&in3, 0);
            gpio_pin_set_dt(&in4, 1);
            break;

        case 'l':  //left
            gpio_pin_set_dt(&in1,0 );
            gpio_pin_set_dt(&in2, 1);
            gpio_pin_set_dt(&in3, 1);
            gpio_pin_set_dt(&in4, 0);
            break;

        case 'h':  //halt
            gpio_pin_set_dt(&in1, 0);
            gpio_pin_set_dt(&in2, 0);
            gpio_pin_set_dt(&in3, 0);
            gpio_pin_set_dt(&in4, 0);
            break;

        default: //you may add break here in default in case the system doesnt perform correctly
            gpio_pin_set_dt(&in1, 0);
            gpio_pin_set_dt(&in2, 0);
            gpio_pin_set_dt(&in3, 0);
            gpio_pin_set_dt(&in4, 0);

    }
}



void set_Speed(double m1_speed, double m2_speed, const struct motors *motors){
    // uint32_t NUM_STEPS = 50;
    // uint32_t step = motors.period / NUM_STEPS;
    // pwm_set_pulse_dt(&(motors->enA), (motors.period/NUM_STEPS)*(m1_speed));  //giving this a duty cycle
    pwm_set_pulse_dt(&(motors->enA),m1_speed); 
    pwm_set_pulse_dt(&(motors->enB),m2_speed);
    //k_mutex_lock(&uart_mutex, K_FOREVER);
    printk("Motor Speeds Set\n");
    //k_mutex_unlock(&uart_mutex);
    //period= 0.0001 sec
    // pwm_set_dt(motors->enB, 255, m2_speed);

    //

}

void motor_chalao(void){

    int percent_to_period_A = 0;
    int percent_to_period_B = 0;
    percent_to_period_A = 0.5*motor.enA.period;
    percent_to_period_B =  0.5*motor.enB.period;
    printk("Motor thread runnig\n");
    set_Speed(percent_to_period_A, percent_to_period_B, &motor);
    printk("Motor direction setting\n");
    setMotorDirection('f');
    // gpio_pin_set_dt(&in1, 0);
    //gpio_pin_set_dt(&in2, 1);
    k_sleep(K_SECONDS(3));
}

/* size of stack area used by each thread */
#define STACKSIZE 2048

/* scheduling priority used by each thread */
#define PRIORITY 7
K_THREAD_DEFINE(motors_id, STACKSIZE, motor_chalao, NULL, NULL, NULL, PRIORITY, 0, 0);