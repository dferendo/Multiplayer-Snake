//
// Created by dylan on 11/04/2017.
//
#ifndef SNAKES_FOOD_H
#define SNAKES_FOOD_H

#include "Snake.h"

/**
 * The type of the food.
 */
typedef enum Type {
    F_NORMAL = 0
} Type;

/**
 * The position of the food and type.
 */
typedef struct Food {
    Position * position;
    Type foodType;
} Food;

/**
 * A pthread that generates a food at random intervals. The interval is specified in
 * {@link GameSetting.h}. Also, the maximum amount of food at a time is also
 * specified {@link GameSetting.h}. The thread will terminate once the boolean
 * variable passed as a parameter is changed.
 *
 * @param arg: FoodGeneratorParams is passed as an argument.
 * @return: Nothing.
 */
void * generateFood(void * arg);

#endif //SNAKES_FOOD_H
