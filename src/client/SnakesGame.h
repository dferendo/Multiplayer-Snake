//
// Created by dylan on 17/04/2017.
//

#ifndef SNAKES_SNAKESGAME_H
#define SNAKES_SNAKESGAME_H

#include <ncurses.h>
#include "../utility/Vector.h"
#include "../server/Snake.h"
typedef struct ReadUserInputThreadParams {
    int sockFd;
} ReadUserInputThreadParams;

void gameInit(int sockFd);

void gameRunning(int sockFd);

bool foodHandler(int sockFd, Vector * foods, Vector * snakes);

bool snakeHandler(int sockFd, Vector * foods, Vector * snakes);

void clearFoodsVector(Vector * foods);

void clearSnakeVector(Vector * snakes);

void * readDirectionFromUser(void *args);

#endif //SNAKES_SNAKESGAME_H
