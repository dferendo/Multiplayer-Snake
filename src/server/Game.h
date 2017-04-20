//
// Created by dylan on 18/04/2017.
//

#ifndef SNAKES_GAME_H
#define SNAKES_GAME_H

#include "../utility/Vector.h"
#include "../server/Server.h"

typedef struct SnakeWorkerParams {
    Vector * connections;
    Vector * foods;
    Snake * snake;
} SnakeWorkerParams;

typedef enum {
    WINNER = 0,
    DIED = 1,
    NORMAL = 2
} SnakeStatus;

typedef struct SnakeWorkerReturn {
    SnakeStatus status;
} SnakeWorkerReturn;

void gameInitialize();

void gameLoop();

void sendSnakeInformationToClients();

bool createSnakeWorkers();

void * checkForChangeOfDirections(void * args);

void sendEndGameToClients(int sockFd, SnakeStatus status);

#endif //SNAKES_GAME_H
