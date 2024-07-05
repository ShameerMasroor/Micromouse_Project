#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

typedef struct
{
   uint32_t left_encoder_count;
   uint32_t right_encoder_count;
   const struct gpio_dt_spec left_encoder_pin;
   const struct gpio_dt_spec right_encoder_pin;
   
} encoders_t;

void init_encoders();
double speed_matcher_left();
double speed_matcher_right();
double general_speed_matcher();