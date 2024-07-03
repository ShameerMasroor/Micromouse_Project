#include "../include/motor.h"
// #include "../include/shared_mutex.h"  // Include the shared header file
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

#define STACKSIZE 2048
#define PRIORITY 7

K_MSGQ_DEFINE(motor_control_q, sizeof(char), 10, 1);

#define IN1 DT_ALIAS(in1)
#define IN2 DT_ALIAS(in2)
#define IN3 DT_ALIAS(in3)
#define IN4 DT_ALIAS(in4)

static const struct gpio_dt_spec in1 = GPIO_DT_SPEC_GET(IN1, gpios);
static const struct gpio_dt_spec in2 = GPIO_DT_SPEC_GET(IN2, gpios);
static const struct gpio_dt_spec in3 = GPIO_DT_SPEC_GET(IN3, gpios);
static const struct gpio_dt_spec in4 = GPIO_DT_SPEC_GET(IN4, gpios);

static motors motor = {
    .enA = PWM_DT_SPEC_GET(DT_ALIAS(pwm_m1)),
    .enB = PWM_DT_SPEC_GET(DT_ALIAS(pwm_m2)),
};

static char command;

float percent_to_period_A = 0;
float percent_to_period_B = 0;

void init_motors(void)
{
    int ret;

    ret = gpio_pin_configure_dt(&in1, GPIO_OUTPUT);
    if (ret < 0) {
        // k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure in1 pin\n", ret);
        // k_mutex_unlock(&uart_mutex);
        return; 
    }
    ret = gpio_pin_configure_dt(&in2, GPIO_OUTPUT);
    if (ret < 0) {
        // k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure in2 pin\n", ret);
        // k_mutex_unlock(&uart_mutex);
        return;
    }

    ret = gpio_pin_configure_dt(&in3, GPIO_OUTPUT);
    if (ret < 0) {
        // k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure in3 pin\n", ret);
        // k_mutex_unlock(&uart_mutex);
        return;
    }

    ret = gpio_pin_configure_dt(&in4, GPIO_OUTPUT);
    if (ret < 0) {
        // k_mutex_lock(&uart_mutex, K_FOREVER);
        printk("Error %d: failed to configure in4 pin\n", ret);
        // k_mutex_unlock(&uart_mutex);
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

    // k_mutex_lock(&uart_mutex, K_FOREVER);
    printk("Both motors initialized\n");
    // k_mutex_unlock(&uart_mutex);
}

void setMotorDirection(char direction)
{
    switch(direction)
    {
        case 'f':  //forwards
            gpio_pin_set_dt(&in1, 1);
            gpio_pin_set_dt(&in2, 0);
            gpio_pin_set_dt(&in3, 1);
            gpio_pin_set_dt(&in4, 0);
            printk("Going forwards\n");
            break;

        case 'b':  //backwards
            gpio_pin_set_dt(&in1, 0);
            gpio_pin_set_dt(&in2, 1);
            gpio_pin_set_dt(&in3, 1);
            gpio_pin_set_dt(&in4, 0);
            printk("Going backwards\n");
            break;

        case 'r':  //right
            gpio_pin_set_dt(&in1, 1);
            gpio_pin_set_dt(&in2, 0);
            gpio_pin_set_dt(&in3, 0);
            gpio_pin_set_dt(&in4, 1);
            printk("Going right\n");
            break;

        case 'l':  //left
            gpio_pin_set_dt(&in1,0 );
            gpio_pin_set_dt(&in2, 1);
            gpio_pin_set_dt(&in3, 1);
            gpio_pin_set_dt(&in4, 0);
            printk("Going left\n");
            break;

        case 'h':  //halt
            gpio_pin_set_dt(&in1, 0);
            gpio_pin_set_dt(&in2, 0);
            gpio_pin_set_dt(&in3, 0);
            gpio_pin_set_dt(&in4, 0);
            printk("HALT!!!!\n");
            break;

        default:
            gpio_pin_set_dt(&in1, 0);
            gpio_pin_set_dt(&in2, 0);
            gpio_pin_set_dt(&in3, 0);
            gpio_pin_set_dt(&in4, 0);
    }
}

void set_Speed(float m1_speed, float m2_speed, const motors *motors)
{
    pwm_set_pulse_dt(&(motors->enA),m1_speed); 
    pwm_set_pulse_dt(&(motors->enB),m2_speed);
    printk("Motor Speeds Set\n");
}

void motor_thread(void)
{
    init_motors();
    percent_to_period_A = 0.45*motor.enA.period;
    percent_to_period_B = 0.5*motor.enB.period;
    set_Speed(percent_to_period_A, percent_to_period_B, &motor);
    while (1)
    {
        k_msgq_get(&motor_control_q, &command, K_FOREVER); 

        // printk("Motor thread running\n");

        // printk("Motor direction setting\n");
        setMotorDirection(command);
    }

    k_sleep(K_MSEC(1));
}

K_THREAD_DEFINE(motors_id, STACKSIZE, motor_thread, NULL, NULL, NULL, PRIORITY, 0, 0);