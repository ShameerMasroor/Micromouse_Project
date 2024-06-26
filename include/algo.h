#ifndef ALGO_H
#define ALGO_H
#include <stdbool.h>

typedef struct {
    int rows;
    int cols;
    int maze_map[20][30][4];  // Assuming max maze size, 4 for direction walls
    int end_row;
    int end_col;
    int start_row;
    int start_col;
} Maze;

typedef struct { // output direction
    char forward;
    char left;
    char back;
    char right;
    //why not a single
} Direction;

typedef struct{ //input sensor information
    bool left;
    bool front;}
Detection;

void wallFollower(Maze*, char* path, Detection* detect);

// why is wall follower taking path? shouldn't it be out
void optimizePath(char* path, char* optimized_path);
//same question

#endif