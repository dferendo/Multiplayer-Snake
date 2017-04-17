//
// Created by dylan on 11/04/2017.
//

#ifndef SNAKES_SNAKE_H
#define SNAKES_SNAKE_H

#include <ncurses.h>
#include "../utility/LinkedList.h"
#include "../utility/Vector.h"

typedef enum {
    D_UP,
    D_LEFT,
    D_DOWN,
    D_RIGHT
} Direction;

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct {
    int size;
    Direction direction;
    LinkedListPosition * positions;
} Snake;

void snakesInit(Vector * connections);

#endif //SNAKES_SNAKE_H
