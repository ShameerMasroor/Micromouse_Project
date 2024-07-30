#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include "IR.h"
extern struct k_timer read_speed_timer;


typedef struct
{
   uint32_t left_encoder_count;
   uint32_t right_encoder_count;
   uint32_t rotation_left_counter;
   uint32_t rotation_right_counter;
   const struct gpio_dt_spec left_encoder_pin;
   const struct gpio_dt_spec right_encoder_pin;
   
} encoders_t;


void init_encoders();
double speed_matcher_left();
double speed_matcher_right();
double general_speed_matcher();
void speed_detector();
int distance();

int return_left_enc_count();
int return_right_enc_count();
void set_controller(bool flag);

