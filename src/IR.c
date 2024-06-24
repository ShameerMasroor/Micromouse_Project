#include "../include/IR.h"
#include <stdio.h>

void init_IR(const struct ir *IR) {
   int rett;

   // Configure the voltage pin as an output
   rett = gpio_pin_configure_dt(&IR->d_out, GPIO_INPUT);
   
   if (rett < 0) {
       //k_mutex_lock(&uart_mutex, K_FOREVER);
       printf("Error %d: failed to configure voltage pin\n", rett);
       //k_mutex_unlock(&uart_mutex);
       return;
   }

  

   printf("IR sensor initialized\n");
   //k_mutex_unlock(&uart_mutex);
}



bool wall_detected(const struct ir *IR){

   bool wall =0;

   wall = gpio_pin_get(IR-> d_out.port, IR->d_out.pin);

   if (wall)
       printf( IR->dir, "Wall detected");

   else
       printf(IR->dir,"Wall absent");

   return wall;

}