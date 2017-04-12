//
// Created by dylan on 11/04/2017.
//

#include <curses.h>

#ifndef SNAKES_FOOD_H
#define SNAKES_FOOD_H

#include "Snake.h"

#define MAXIMUM_FOOD_INTERVAL_SECS 3

typedef enum {
    NORMAL = 'o'
} Type;

// Food position will be unique.
typedef struct {
    Position * position;
    Type foodType;
} Food;

void generateFood(WINDOW * window);

bool foodEquals(Food * f1, Food * f2);

#endif //SNAKES_FOOD_H
