#include "../include/IR.h"

void init_IR(ir *IR) {
   int rett;

   // Configure the voltage pin as an output
   rett = gpio_pin_configure_dt(&IR->d_out, GPIO_INPUT);
   
   if (rett < 0) 
   {
    printk("Error %d: failed to configure voltage pin\n", rett);
    return;
   }

   printk("IR sensor initialized\n");
}



bool read_IR(ir *IR)
{
   bool wall = gpio_pin_get(IR-> d_out.port, IR->d_out.pin);

   if (wall)
       printk(IR->num, " Wall detected");
   else
       printk(IR->num," Wall absent");

   return wall;
}