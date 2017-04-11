//
// Created by dylan on 11/04/2017.
//

#ifndef SNAKES_SNAKE_H
#define SNAKES_SNAKE_H

#include <ncurses.h>
#include "../Utility/LinkedList.h"

#define DEFAULT_START_DIRECTION D_UP
#define DEFAULT_START_SIZE 1

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

Snake * createSnake(WINDOW * window);

int moveSnake(WINDOW * window, Snake * snake);

int growSnake(WINDOW * window, Snake * snake);

#endif //SNAKES_SNAKE_H
