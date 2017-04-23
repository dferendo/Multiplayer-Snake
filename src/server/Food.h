//
// Created by dylan on 11/04/2017.
//
#ifndef SNAKES_FOOD_H
#define SNAKES_FOOD_H

#include "Snake.h"

typedef enum Type {
    F_NORMAL = 0
} Type;

typedef struct Food {
    Position * position;
    Type foodType;
} Food;

void * generateFood(void * arg);

#endif //SNAKES_FOOD_H
