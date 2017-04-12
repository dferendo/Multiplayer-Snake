//
// Created by dylan on 12/04/2017.
//

#ifndef SNAKES_VECTOR_H
#define SNAKES_VECTOR_H

#include "../snakes-components/Food.h"

typedef struct vectorFood {
    Food ** foods;
    size_t size;
} vectorFood;

vectorFood * initModifiedVectorFood();

int addFoodToModifiedVector(vectorFood * vector, Food * food);

void deleteVector(vectorFood * vector);

int deleteItem(vectorFood * vector, Food * food);

#endif //SNAKES_VECTOR_H
