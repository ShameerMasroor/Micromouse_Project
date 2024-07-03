#ifndef ALGO_H
#define ALGO_H

#include "sensing.h"

char right_hand_follower(sensor_data_t *received_data);

#endif

//=====================================================================================================
// typedef struct 
// {
//     int rows;
//     int cols;
//     int end_row;
//     int end_col;
//     int start_row;
//     int start_col;
//     // int maze_map[20][30][4];  // Assuming max maze size, 4 for direction walls
// } Maze;

// typedef struct 
// { // output direction
//     char forward;
//     char left;
//     char back;
//     char right;
//     //why not a single
// } Direction;

// void wallFollower(Maze*, char* path, sensor_data_t *sensor_data);
// void optimizePath(char* path, char* optimized_path);