//
// Created by dylan on 11/04/2017.
//
#ifndef SNAKES_FOOD_H
#define SNAKES_FOOD_H

#include "Snake.h"
#include "CharactersDefinition.h"
#include <curses.h>

#define MAXIMUM_FOOD_INTERVAL_SECS 3

typedef struct vectorFood vectorFood;

// Food position will be unique for every point.
typedef struct Food {
    Position * position;
    Type foodType;
} Food;

vectorFood * allFood;

void foodInit(WINDOW * window);

void generateFood(WINDOW * window);

void render(WINDOW * window);

bool foodEquals(Food * f1, Food * f2);

#endif //SNAKES_FOOD_H
