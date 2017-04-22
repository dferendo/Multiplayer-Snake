//
// Created by dylan on 18/04/2017.
//

#ifndef SNAKES_GAME_H
#define SNAKES_GAME_H

#include "../utility/Vector.h"
#include "../server/Server.h"
#include "Snake.h"

typedef struct ChangeDirectionParams {
    Vector * connections;
    bool * killThread;
} ChangeDirectionParams;

typedef struct FoodGeneratorParams {
    Vector * foods;
    Vector * connections;
    bool * killThread;
} FoodGeneratorParams;

typedef enum {
    WINNER = 0,
    DIED = 1,
    NORMAL = 2,
    RESTART = 3
} SnakeStatus;

void * gameInitialize(void *);

void gameLoop(Vector *connections, Vector *foods);

void clearDataUsedForGame(Vector *connections, Vector *foods);

bool moveSnakes(Vector *connections, Vector *foods);

#endif //SNAKES_GAME_H
