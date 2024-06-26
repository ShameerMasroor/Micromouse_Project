#include "../include/IR.h"struct

void init_IR(const ir *IR) {
   int rett;

   // Configure the voltage pin as an output
   rett = gpio_pin_configure_dt(&IR->d_out, GPIO_INPUT);
   
   if (rett < 0) 
   {
    printf("Error %d: failed to configure voltage pin\n", rett);
    return;
   }

   printf("IR sensor initialized\n");
}



bool wall_detected(const ir *IR)
{
   bool wall = gpio_pin_get(IR-> d_out.port, IR->d_out.pin);

   if (wall)
       printf(IR->dir, "Wall detected");
   else
       printf(IR->dir,"Wall absent");

   return wall;
}