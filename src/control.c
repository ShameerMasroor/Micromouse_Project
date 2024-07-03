#include "../include/control.h"


#define STACK_SIZE 2048
#define PRIORITY 7
K_MUTEX_DEFINE(uart_mutex);


static Maze maze  = {
    .cols = 16,
    .rows = 16,
    .start_col= 1,
    .end_row= 16,
    .end_col= 16,
    .start_row= 1,
};
static char path[100];
static char  optimized_path[100];


// extern struct k_msgq <name>; 

static sensor_data_t sensor_data =
    {
        .ir_data = NULL
    };


void control_thread(void)
{


    while (1) {
        /* get a data item */
        k_msgq_get(&sensing_control_q, &sensor_data, K_FOREVER);
        printk("Rcvd in control \n ");
        
        if(sensor_data.ir_data.left_ir_data){
            printk(" left data here \n"); 
        }

        right_hand_follower(&sensor_data);
        k_sleep(K_SECONDS(1)); 
        }

        
    
}

void chalja_algo(void){
    while(1){
    right_hand_follower(&sensor_data);
    k_sleep(K_SECONDS(1));
    }
}

int main(void) {
    printk("Micromouse Robot Starting...\n");
    // init_motors();
    // wallFollower(&maze, path, &sensor_data);
    //optimizePath(path, optimized_path);
    return 0;
}

// K_THREAD_DEFINE(algo_thread_id, STACK_SIZE, chalja_algo, NULL, NULL, NULL, 14, 0, 0);
K_THREAD_DEFINE(control_thread_id, STACK_SIZE, control_thread, NULL, NULL, NULL, 10, 0, 0);

