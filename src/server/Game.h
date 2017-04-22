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
} ChangeDirectionParams;

typedef struct FoodGeneratorParams {
    Vector * foods;
    Vector * connections;
} FoodGeneratorParams;

typedef enum {
    WINNER = 0,
    DIED = 1,
    NORMAL = 2
} SnakeStatus;

void * gameInitialize(void *);

void gameLoop(Vector *connections, Vector *foods, pthread_mutex_t lock);

bool moveSnakes(Vector *connections, Vector *foods);

#endif //SNAKES_GAME_H
