//
// Created by dylan on 12/04/2017.
//

#ifndef SNAKES_VECTOR_H
#define SNAKES_VECTOR_H

#include <stdlib.h>

typedef struct Vector {
    void ** data;
    size_t size;
} Vector;

Vector * initVector();

int addItemToVector(Vector *vector, void *item);

void deleteVector(Vector * vector);

int deleteItemFromVector(Vector *vector, void *item);

#endif //SNAKES_VECTOR_H
