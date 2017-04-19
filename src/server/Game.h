//
// Created by dylan on 18/04/2017.
//

#ifndef SNAKES_GAME_H
#define SNAKES_GAME_H

#include "../utility/Vector.h"

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

void sendSnakeInformationToClients();

void createSnakeWorkers();

bool checkWinners();

Position * moveHeadSnake(Direction direction, Position *position);

void * snakeAction(void * args);

bool checkHeadCollision(Snake * snake, Vector * connections);

void checkIfNextPositionIsFoodAndGrow(Snake *snake, Vector *foods);

bool checkIfNextPositionIsCollision(Snake * snake, Vector * connections);

void snakeMove(Snake * snake);

#endif //SNAKES_GAME_H
