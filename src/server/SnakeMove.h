//
// Created by dylan on 19/04/2017.
//

#ifndef SNAKES_SNAKEMOVE_H
#define SNAKES_SNAKEMOVE_H

#include "Snake.h"

Position * moveHeadSnake(Direction direction, Position *position);

void * snakeAction(void * args);

bool checkHeadCollision(Snake * snake, Vector * connections);

bool checkIfNextPositionIsFoodAndGrow(Snake *snake, Vector *foods);

bool checkIfNextPositionIsCollision(Snake * snake, Vector * connections);

void snakeMove(Snake * snake);

#endif //SNAKES_SNAKEMOVE_H
