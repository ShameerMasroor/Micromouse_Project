#include "../include/IR.h"
#include <stdbool.h>


#define IR_OUT DT_ALIAS(dtout)
#define IR_OUT_2 DT_ALIAS(dtout2)

#if !DT_NODE_HAS_STATUS(IR_OUT, okay)
#error "Unsupported board: IR 1 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(IR_OUT_2, okay)
#error "Unsupported board: IR 2 devicetree alias is not defined"
#endif


static const struct gpio_dt_spec left_pin = GPIO_DT_SPEC_GET(IR_OUT, gpios);
static const struct gpio_dt_spec front_pin = GPIO_DT_SPEC_GET(IR_OUT_2, gpios);


static ir ir_left={.d_out=left_pin};
// static ir ir_right{.d_out=pin, .position='r'};
static ir ir_front={.d_out=front_pin};
// static ir ir_back{.d_out=pin, .position='b'};
static ir_data_t ir_data;


void init_IR(void) {
    int rett;
   // Configure the voltage pin as an output
   rett = gpio_pin_configure_dt(&ir_left.d_out, GPIO_INPUT);
   
   if (rett < 0) 
   {
    printk("Error %d: failed to configure IR_left pin\n", rett);
    return;
   }

   rett = gpio_pin_configure_dt(&ir_front.d_out, GPIO_INPUT);
   
   if (rett < 0) 
   {
    printk("Error %d: failed to configure IR_front pin\n", rett);
    return;
   }


   printk("IR sensors initialized\n");
}

ir_data_t read_IR(void)
{
    bool wall_left = gpio_pin_get_dt(&ir_left.d_out);
    // printf( wall_left);
    // if (wall_left)
    //     printk("Left Wall detected\n");
    //  else
    //     printk("Left Wall absent\n");

    bool wall_front = gpio_pin_get_dt(&ir_front.d_out);
    // if (wall_front)
    //     printk("Front Wall detected\n");
    // else
    //     printk("Front Wall absent\n");

    ir_data.left_ir_data = wall_left;
    ir_data.front_ir_data =wall_front;

    return ir_data;
    

}



