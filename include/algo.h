#ifndef ALGO_H
#define ALGO_H

typedef struct {
    int rows;
    int cols;
    int maze_map[20][30][4];  // Assuming max maze size, 4 for direction walls
} Maze;

typedef struct {
    char forward;
    char left;
    char back;
    char right;
} Direction;

void wallFollower(Maze*, char* path);
void optimizePath(char* path, char* optimized_path);

#endif