#include "../include/IR.h"
#include <stdio.h>

void init_IR(const struct ir *sensor) {
   int rett;

   // Configure the voltage pin as an output
   rett = gpio_pin_configure_dt(&sensor->d_out, GPIO_INPUT);
   
   if (rett < 0) {
       //k_mutex_lock(&uart_mutex, K_FOREVER);
       printf("Error %d: failed to configure voltage pin\n", rett);
       //k_mutex_unlock(&uart_mutex);
       return;
   }

  

   printf("IR sensor initialized\n");
   //k_mutex_unlock(&uart_mutex);
}



bool wall_detected(const struct ir *sensor){

   bool wall =0;

   wall = gpio_pin_get(sensor-> d_out.port, sensor->d_out.pin);

   if (wall){ 
       printf("Wall detected");
   }

   else {
       printf("Wall absent");
   }

   return wall;

}