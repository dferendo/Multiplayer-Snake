//
// Created by dylan on 11/04/2017.
//
#ifndef SNAKES_FOOD_H
#define SNAKES_FOOD_H

#include "Snake.h"

typedef enum {
    F_NORMAL = 0
} Type;

typedef struct Food {
    Position * position;
    Type foodType;
} Food;

static const chtype stringFromFood(Type type) {
    const chtype TYPE_STRINGS[] = { 'o' };
    return TYPE_STRINGS[type];
}

void * generateFood(void * arg);

void writeFoodDataToClients(Vector * connections, Vector * foods);

#endif //SNAKES_FOOD_H
