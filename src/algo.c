#include "algo.h"
#include "motor.h"
#include "sensing.h"

typedef struct {
    char forward;
    char right;
    char back;
    char left;
} Direction;

Direction direction = {'N', 'E', 'S', 'W'};

void Rotate_Clockwise() {
    char temp = direction.right;
    direction.right = direction.back;
    direction.back = direction.left;
    direction.left = direction.forward;
    direction.forward = temp;
    // Assume a function to rotate the robot 90 degrees clockwise
    rotateClockwise();
}

void Rotate_CounterClockwise() {
    char temp = direction.forward;
    direction.forward = direction.left;
    direction.left = direction.back;
    direction.back = direction.right;
    direction.right = temp;
    // Assume a function to rotate the robot 90 degrees counterclockwise
    rotateCounterClockwise();
}

void moveForward(int* row, int* col) {
    if (direction.forward == 'E') {
        (*col)++;
    } else if (direction.forward == 'W') {
        (*col)--;
    } else if (direction.forward == 'N') {
        (*row)--;
    } else if (direction.forward == 'S') {
        (*row)++;
    }
    setMotorDirection(FORWARD);
    k_sleep(K_MSEC(500)); // Assume it takes 500 ms to move forward by one cell
    stopMotors();
}

void wallFollower(Maze* m, char* path) {
    int row = m->start_row;
    int col = m->start_col;
    int idx = 0;

    while (!(row == m->end_row && col == m->end_col)) {
        bool left_wall = isWallLeft();
        bool front_wall = isWallFront();

        if (!left_wall) {
            Rotate_CounterClockwise();
            moveForward(&row, &col);
            path[idx++] = direction.forward;
        } else if (!front_wall) {
            moveForward(&row, &col);
            path[idx++] = direction.forward;
        } else {
            Rotate_Clockwise();
        }
    }
    path[idx] = '\0';
}

void optimizePath(char* path, char* optimized_path) {
    int idx = 0;
    for (int i = 0; path[i] != '\0'; i++) {
        if ((path[i] == 'E' && path[i + 1] == 'W') || 
            (path[i] == 'W' && path[i + 1] == 'E') || 
            (path[i] == 'N' && path[i + 1] == 'S') || 
            (path[i] == 'S' && path[i + 1] == 'N')) {
            i++;
        } else {
            optimized_path[idx++] = path[i];
        }
    }
    optimized_path[idx] = '\0';
}

int main() {
    Maze maze = {10, 10, 0, 0, 9, 9}; // Example maze dimensions and start/end points
    char path[100];
    char optimized_path[100];

    wallFollower(&maze, path);
    optimizePath(path, optimized_path);

    printf("Path: %s\n", path);
    printf("Optimized Path: %s\n", optimized_path);

    return 0;
}
