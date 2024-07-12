#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/sensing.h"
#include "../include/algo.h"

char right_hand_follower(sensor_data_t *received_data)
{
    bool ir_right_state = received_data->ir_data.left_ir_data;
    bool ir_front_state = received_data->ir_data.front_ir_data;

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

//==============================================================================

//==============================================================================

#define SIZE 16
point Stack[SIZE];


int top_index = -1;
int rows, cols = 6, 6;
int cells[rows][cols];
int flood[rows][cols] = {{4, 3, 2, 2, 3, 4},
                         {3, 2, 1, 1, 2, 3},
                         {2, 1, 0, 0, 1, 2},
                         {2, 1, 0, 0, 1, 2},
                         {3, 2, 1, 1, 2, 3},
                         {4, 3, 2, 2, 3, 4}};
int md{0};
typedef enum {
    N,
    E,
    S,
    W
} Orientation;

Orientation Orient = N;
static point Position = {0,0};
int ds{100};
int dn{100};
int de{100};
int dw{100};

void push(const point element)
{
    top_index++;
    Stack[top_index] = element;
}

point pop()
{
    if (!isempty())
    {
        point element = Stack[top_index];
        top_index--;
        return element;
    }
    return point(-1,-1);
}

bool isempty()
{
    return (top_index == -1);
}

char getTurn(point location, Orientation current, Orientation newDirection) 
{
    int diff = (newDirection - current + 4) % 4;
    switch (diff) 
    {
        case 0:
        location.y ++;
        return 'f';

        case 1:
        location.x ++;
        return 'r';
        
        case 2:
        location.y --;
        return 'b';

        case 3:
        location.x --;
        return 'l';

        default:
        return 'h';  // Default case, should not happen
    }
}

// Function to update walls
void updateWalls(point location, Orientation orient, sensor_data_t *recieved_data) {
    bool left_wall = received_data->ir_data.left_ir_data;
    bool right_wall = received_data->ir_data.right_ir_data;
    bool front_wall = received_data->ir_data.front_ir_data;

    if (left_wall && right_wall && front_wall)
    {
        switch (orient)
        {
            case 0: cells[location.y][location.x] = 13; break;
            case 1: cells[location.y][location.x] = 12; break;
            case 2: cells[location.y][location.x] = 11; break;
            case 3: cells[location.y][location.x] = 14; break;
        }
    } 
    else if (left_wall && right_wall && !front_wall) 
    {
        if (orient == 0 || orient == 2)
            cells[location.y][location.x] = 9;
        else if (orient == 1 || orient == 3)
            cells[location.y][location.x] = 10;
    }
    else if (left_wall && front_wall && !right_wall) 
    {
        switch (orient) 
        {
            case 0: cells[location.y][location.x] = 8; break;
            case 1: cells[location.y][location.x] = 7; break;
            case 2: cells[location.y][location.x] = 6; break;
            case 3: cells[location.y][location.x] = 5; break;
        }
    } 
    else if (right_wall && front_wall && !left_wall)
    {
        switch (orient)
        {
            case 0: cells[location.y][location.x] = 7; break;
            case 1: cells[location.y][location.x] = 6; break;
            case 2: cells[location.y][location.x] = 5; break;
            case 3: cells[location.y][location.x] = 8; break;
        }
    } 
    else if (front_wall) 
    {
        switch (orient) 
        {
            case 0: cells[location.y][location.x] = 2; break;
            case 1: cells[location.y][location.x] = 3; break;
            case 2: cells[location.y][location.x] = 4; break;
            case 3: cells[location.y][location.x] = 1; break;
        }
    }
    else if (left_wall) 
    {
        switch (orient) 
        {
            case 0: cells[location.y][location.x] = 1; break;
            case 1: cells[location.y][location.x] = 2; break;
            case 2: cells[location.y][location.x] = 3; break;
            case 3: cells[location.y][location.x] = 4; break;
        }
    } 
    else if (right_wall) 
    {
        switch (orient) 
        {
            case 0: cells[location.y][location.x] = 3; break;
            case 1: cells[location.y][location.x] = 4; break;
            case 2: cells[location.y][location.x] = 1; break;
            case 3: cells[location.y][location.x] = 2; break;
        }
    }
}

Neighbours get_neighbours(point location)
{
    Neighbours neighbours;
    if (x > 0)
    {
        neighbours.West.x = location.x - 1;
        neighbours.West.y = location.y;
    }
    if (x < (cols - 1))
    {
        neighbours.East.x = location.x + 1;
        neighbours.East.y = location.y;
    }
    if (y > 0)
    {
        neighbours.South.x = location.x;
        neighbours.South.y = location.y - 1;
    }
    if (y < (rows - 1))
    {
        neighbours.North.x = location.x;
        neighbours.North.y = location.y + 1;
    }
    return neighbours;
}

bool pushable(point location)
{
    return !((location.x == 3 || location.x == 2) && (location.y == 2 || location.y == 3));
}

// Function to check if the mouse can move from current location (x, y) to its neighbours
bool isAccessible(point location, char direction) {
    if (direction == 'S')
        return (cells[location.y][location.x] == 4 || cells[location.y][location.x] == 5 || cells[location.y][location.x] == 6 || 
            cells[location.y][location.x] == 10 || cells[location.y][location.x] == 11 || cells[location.y][location.x] == 12 || 
            cells[location.y][location.x] == 14)
    else if (direction == 'N')
        return (cells[location.y][location.x] == 2 || cells[location.y][location.x] == 7 || cells[location.y][location.x] == 8 || 
                cells[location.y][location.x] == 10 || cells[location.y][location.x] == 12 || cells[location.y][location.x] == 13 || 
                cells[location.y][location.x] == 14)
    else if (direction == 'W')
        return (cells[location.y][location.x] == 1 || cells[location.y][location.x] == 5 || cells[location.y][location.x] == 8 || 
                cells[location.y][location.x] == 9 || cells[location.y][location.x] == 11 || cells[location.y][location.x] == 13 || 
                cells[location.y][location.x] == 14)
    else if (direction == 'E') 
        return (cells[location.y][location.x] == 3 || cells[location.y][location.x] == 6 || cells[location.y][location.x] == 7 || 
                cells[location.y][location.x] == 9 || cells[location.y][location.x] == 11 || cells[location.y][location.x] == 12 || 
                cells[location.y][location.x] == 13)
}

void get_md(point location)
{
    Neighbours neighbours = get_neighbours(location);
    
    if isAccessible(location, 'S')
    {
        ds = flood[neighbours.South.y][neighbours.South.x];
        if (ds < md)
            md = ds;
    }
    if isAccessible(location, 'N')
    {
        dn = flood[neighbours.North.y][neighbours.North.x];
        if (dn < md)
            md = dn;
    }
    if isAccessible(location, 'W')
    {
        dw = flood[neighbours.West.y][neighbours.West.x];
        if (dw < md)
            md = dw;
    }
    if isAccessible(location, 'E')
    {
        de = flood[neighbours.East.y][neighbours.East.x];
        if (de < md)
            md = de;
    }
}

bool check_distance(point location)
{
    get_md(location);
    return (md != (flood[location.y][location.x] - 1))
}

void populate(void)
{
    flood[location.y][location.x] = md + 1;
    if (pushable(neighbours.East))
        push(neighbours.East);
    if (pushable(neighbours.West))
        push(neighbours.West);
    if (pushable(neighbours.North))
        push(neighbours.North);
    if (pushable(neighbours.South))
        push(neighbours.South);   
} 

char floodfill(sensor_data_t *recieved_data)
{
    push(Position);
    updateWalls(Position, Orient, recieved_data);
    while (!isempty(Stack))
    {
        point loc = pop();
        printf("Location: (%d , %d )", loc.x, loc.y);
        if (check_distance(loc))
            populate();
    }

    get_md(Position);
    if (md == dn)
    {
        return getTurn(Position,Orient, N);
        Orient = N;
    }
    else if (md == dw)
    {
        return getTurn(Position, Orient, W);
        Orient = W;
    }
    else if (md == de)
    {
        return getTurn(Position, Orient, E);
        Orient = E;
    }
    else
    {
        return getTurn(Position, Orient, S);
        Orient = S;
    }

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