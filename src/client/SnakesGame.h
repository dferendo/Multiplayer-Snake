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

void gameInit(Vector * connections, int sockFd);

void gameRunning(Vector * connections, Vector * foods, int sockFd);

int readDelimiterSnakes(int socketFd);

Vector * readFoodsFromSocket(int socketFileDescriptor);

void readSnakesFromSocket(Vector * connections, int sockFd);

Snake * clearPreviousSnakeForNewerSnake(Vector * connections, char * name);

void clearFoodsVector(Vector * foods);

WINDOW * displayNewData(Vector * foods, Vector * connections);

void * readDirectionFromUser(void *args);

void sendUserDirection(int sockFd, int direction);

#endif //SNAKES_SNAKESGAME_H
