//
// Created by dylan on 11/04/2017.
//

#include <stdbool.h>
#include "Food.h"

bool foodEquals(Food *f1, Food *f2) {
    if (f1->position->y == f2->position->y &&
            f1->position->x == f2->position->x) {
        return true;
    }
    return false;
}
