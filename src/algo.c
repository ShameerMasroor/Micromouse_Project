#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/sensing.h"
#include "../include/algo.h"

// char right_hand_follower(sensor_data_t *received_data)
// {
//     bool ir_right_state = received_data->ir_data.left_ir_data;
//     bool ir_front_state = received_data->ir_data.front_ir_data;

//     if (ir_right_state && !ir_front_state){
//         printk("Go forward\n");
//         return 'f';
//     }
//     else if (ir_right_state && ir_front_state){
//         printk("Go left\n");
//         return 'l';
//     }
//     else if (!ir_right_state){
//         printk("Go right\n");
//         return 'r';
//     }

//     return 'h';
// }

//==============================================================================

//==============================================================================

#define SIZE 6
point Stack[SIZE];
int cells[SIZE][SIZE];
int flood[SIZE][SIZE] = {{4, 3, 2, 2, 3, 4},
                         {3, 2, 1, 1, 2, 3},
                         {2, 1, 0, 0, 1, 2},
                         {2, 1, 0, 0, 1, 2},
                         {3, 2, 1, 1, 2, 3},
                         {4, 3, 2, 2, 3, 4}};

int top_index = -1;
int md = 100;
int ds = 100;
int dn = 100;
int de = 100;
int dw = 100;

typedef enum {
    N,
    E,
    S,
    W
} Orientation;

Orientation Orient = N;
static point Position = {0,0};

void push(const point element)
{
    top_index++;
    Stack[top_index] = element;
}

bool isempty()
{
    return (top_index == -1);
}

point pop()
{
    if (!isempty())
    {
        point element = Stack[top_index];
        top_index--;
        return element;
    }
    point location = {-1, -1};
    return location;
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
void updateWalls(point location, Orientation orient, sensor_data_t *received_data) {
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
    if (location.x > 0)
    {
        neighbours.West.x = location.x - 1;
        neighbours.West.y = location.y;
    }
    else
    {
        neighbours.West.x = -1;
        neighbours.West.y = -1;
    }
    if (location.x < (SIZE - 1))
    {
        neighbours.East.x = location.x + 1;
        neighbours.East.y = location.y;
    }
    else
    {
        neighbours.East.x = -1;
        neighbours.East.y = -1;
    }
    if (location.y > 0)
    {
        neighbours.South.x = location.x;
        neighbours.South.y = location.y - 1;
    }
    else
    {
        neighbours.South.x = -1;
        neighbours.South.y = -1;
    }
    if (location.y < (SIZE - 1))
    {
        neighbours.North.x = location.x;
        neighbours.North.y = location.y + 1;
    }
    else
    {
        neighbours.North.x = -1;
        neighbours.North.y = -1;
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
            cells[location.y][location.x] == 14);
    else if (direction == 'N')
        return (cells[location.y][location.x] == 2 || cells[location.y][location.x] == 7 || cells[location.y][location.x] == 8 || 
                cells[location.y][location.x] == 10 || cells[location.y][location.x] == 12 || cells[location.y][location.x] == 13 || 
                cells[location.y][location.x] == 14);
    else if (direction == 'W')
        return (cells[location.y][location.x] == 1 || cells[location.y][location.x] == 5 || cells[location.y][location.x] == 8 || 
                cells[location.y][location.x] == 9 || cells[location.y][location.x] == 11 || cells[location.y][location.x] == 13 || 
                cells[location.y][location.x] == 14);
    else 
        return (cells[location.y][location.x] == 3 || cells[location.y][location.x] == 6 || cells[location.y][location.x] == 7 || 
                cells[location.y][location.x] == 9 || cells[location.y][location.x] == 11 || cells[location.y][location.x] == 12 || 
                cells[location.y][location.x] == 13);
}

void get_md(point location)
{
    Neighbours neighbours = get_neighbours(location);
    
    if (isAccessible(location, 'S'))
    {
        ds = flood[neighbours.South.y][neighbours.South.x];
        if (ds < md)
            md = ds;
    }
    if (isAccessible(location, 'N'))
    {
        dn = flood[neighbours.North.y][neighbours.North.x];
        if (dn < md)
            md = dn;
    }
    if (isAccessible(location, 'W'))
    {
        dw = flood[neighbours.West.y][neighbours.West.x];
        if (dw < md)
            md = dw;
    }
    if (isAccessible(location, 'E'))
    {
        de = flood[neighbours.East.y][neighbours.East.x];
        if (de < md)
            md = de;
    }
}

bool check_distance(point location)
{
    get_md(location);
    return (md != (flood[location.y][location.x] - 1));
}

void populate(point location)
{
    Neighbours neighbours = get_neighbours(location);
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

char floodfill(sensor_data_t *received_data)
{
    char command;
    push(Position);
    updateWalls(Position, Orient, received_data);
    while (!isempty(Stack))
    {
        point loc = pop();
        printf("Location: (%d , %d )", loc.x, loc.y);
        if (check_distance(loc))
            populate(loc);
    }

    get_md(Position);
    if (md == dn)
    {
        command  = getTurn(Position,Orient, N);
        Orient = N;
    }
    else if (md == dw)
    {
        command = getTurn(Position, Orient, W);
        Orient = W;
    }
    else if (md == de)
    {
        command = getTurn(Position, Orient, E);
        Orient = E;
    }
    else
    {
        command = getTurn(Position, Orient, S);
        Orient = S;
    }
    return command;
}