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
    bool * keepAlive;
} ReadUserInputThreadParams;

void gameManager(int sockFd);

int gameRunning(int sockFd);

bool foodHandler(int sockFd);

bool snakeHandler(int sockFd);

void clearFoodsVector(Vector * foods);

void clearSnakeVector(Vector * snakes);

void * readDirectionFromUser(void *args);

#endif //SNAKES_SNAKESGAME_H
