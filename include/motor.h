#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    float left_speed;
    float right_speed;
    bool left_direction;
    bool right_direction;
} motor_t;

// Function to initialize the motor control
void initMotorControl(motor_t* motor);

// Function to set the direction of the motors
void setMotorDirection();

// Function to set the speed of the motors
void setMotorSpeed();

// Function to stop the motors
void stopMotors();

#endif
