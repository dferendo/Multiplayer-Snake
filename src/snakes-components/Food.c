//
// Created by dylan on 11/04/2017.
//
#include <stdbool.h>
#include <ncurses.h>
#include <stdlib.h>
#include "Food.h"
#include "../Utility/Vector.h"
#include "../Utility/RandomUtility.h"
#include <unistd.h>

void foodInit(WINDOW * window) {

    allFood = initModifiedVectorFood();

    if (allFood == NULL) {
        return;
    }
    generateFood(window);
}

void generateFood(WINDOW * window) {
    Food * food;

    while (true) {
        // Get position of the new food.
        Position * foodPosition = createNewRandomPosition(window);
        if (foodPosition == NULL) {
            return;
        }
        // Malloc for the new Food
        food = (Food *) malloc(sizeof(Food));

        if (food == NULL) {
            perror("Failed to allocate memory to Food.");
            return;
        }
        // Add variables to the structure
        food->position = foodPosition;
        food->foodType = F_NORMAL;

        addFoodToModifiedVector(allFood, food);
        render(window);
        sleep(MAXIMUM_FOOD_INTERVAL_SECS);
    }
}

bool foodEquals(Food *f1, Food *f2) {
    if (f1->position->y == f2->position->y &&
        f1->position->x == f2->position->x) {
        return true;
    }
    return false;
}

void render(WINDOW * window) {
    for (int i = 0; i < allFood->size; i++) {
        mvwaddch(window, allFood->foods[i]->position->y, allFood->foods[i]->position->x,
                 stringFromFruit(allFood->foods[i]->foodType));
    }
    wrefresh(window);
}
