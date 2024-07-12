#ifndef ALGO_H
#define ALGO_H

#include "sensing.h"

#define RIGHT_HAND 1
#define FLOOD_FILL 2
#define A_STAR 3

#define SELECTED_ALGORITHM FLOOD_FILL

#if SELECTED_ALGORITHM == RIGHT_HAND
char right_hand_follower(sensor_data_t *received_data);
#endif

#if SELECTED_ALGORITHM == FLOOD_FILL

 

typedef struct
{
    int x;
    int y;
} point;

typedef struct{
    point North;
    point South;
    point East;
    point West;
} Neighbours;


char floodfill(sensor_data_t *received_data);
#endif

#if SELECTED_ALGORITHM == A_STAR
void a_star();
#endif

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