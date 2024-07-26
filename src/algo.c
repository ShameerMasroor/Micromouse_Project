#include <stdbool.h>
#include "../include/sensing.h"

int cell = 0;
double rd = 0;
int prev_distance = 0;
char right_hand_follower(sensor_data_t *received_data)
{
    bool ir_right_state = received_data->ir_data.left_ir_data;
    bool ir_front_state = received_data->ir_data.front_ir_data;
    rd = received_data->dist;
    cell = (int)(rd/18.0);
    printk("Cell: %d \n", cell);
    
    printf("Distance: %lf \n", rd);
    if (ir_right_state && !ir_front_state){
        printk("Go forward\n");
        return 'f';
    }
    else if (ir_right_state && ir_front_state){
        printk("Go left\n");
        return 'l';
    }
    else if (!ir_right_state){
        printk("Go right\n");
        return 'r';
    }


    return 'h';
}

//=====================================================================

// #include "../include/algo.h"

// Direction direction = {'N', 'E', 'S', 'W'};

// void Rotate_Clockwise()
// {
//     char temp = direction.right;
//     direction.right = direction.back;
//     direction.back = direction.left;
//     direction.left = direction.forward;
//     direction.forward = temp;
//     // Assume a function to rotate the robot 90 degrees clockwise
//     //rotateClockwise();
//     printk("go right\n");
// }

// void Rotate_CounterClockwise()
// {
//     char temp = direction.forward;
//     direction.forward = direction.left;
//     direction.left = direction.back;
//     direction.back = direction.right;
//     direction.right = temp;
//     // Assume a function to rotate the robot 90 degrees counterclockwise
//     //rotateCounterClockwise();
//     printk("go left\n");
// }

// void moveForward(int* row, int* col)
// {
//     if (direction.forward == 'E') {
//         (*col)++;
//     } else if (direction.forward == 'W') {
//         (*col)--;
//     } else if (direction.forward == 'N') {
//         (*row)--;
//     } else if (direction.forward == 'S') {
//         (*row)++;
//     }
//     // setMotorDirection(FORWARD);
//     // k_sleep(K_MSEC(500)); // Assume it takes 500 ms to move forward by one cell
//     // stopMotors();
//     printk("go forward\n");
// }

// void wallFollower(Maze* m, char* path, sensor_data_t *sensor_data)
// {
//     int row = m->start_row;
//     int col = m->start_col;
//     int idx = 0;

//     if (!(row == m->end_row && col == m->end_col)) {
//         // while () {
//         bool left_wall = sensor_data->ir_data.left_ir_data;
//         bool front_wall = sensor_data->ir_data.front_ir_data;
//         if (!left_wall) {
//             Rotate_CounterClockwise();
//             moveForward(&row, &col);
//             path[idx++] = direction.forward;
//         } else if (!front_wall) {
//             moveForward(&row, &col);
//             path[idx++] = direction.forward;
//         } else {
//             Rotate_Clockwise();
//         }
//     }
//     path[idx] = '\0';
// }

// void optimizePath(char* path, char* optimized_path)
// {
//     int idx = 0;
//     for (int i = 0; path[i] != '\0'; i++) {
//         if ((path[i] == 'E' && path[i + 1] == 'W') || 
//             (path[i] == 'W' && path[i + 1] == 'E') || 
//             (path[i] == 'N' && path[i + 1] == 'S') || 
//             (path[i] == 'S' && path[i + 1] == 'N')) {
//             i++;
//         } else {
//             optimized_path[idx++] = path[i];
//         }
//     }
//     optimized_path[idx] = '\0';
// }