//
// Created by dylan on 11/04/2017.
//

#ifndef SNAKES_SNAKE_H
#define SNAKES_SNAKE_H

#include <ncurses.h>
#include "../utility/LinkedList.h"
#include "../utility/Vector.h"

typedef enum {
    D_UP = 0,
    D_LEFT = 1,
    D_DOWN = 2,
    D_RIGHT = 3
} Direction;

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct {
    Direction direction;
    int size;
    LinkedListPosition * positions;
} Snake;

Snake * createSnake(Vector * initialPositions);

bool checkIfPositionsAreEqual(Position * position1, Position * position2);

#endif //SNAKES_SNAKE_H
