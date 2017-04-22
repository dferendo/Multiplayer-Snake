//
// Created by dylan on 12/04/2017.
//

#include <stdlib.h>
#include <memory.h>
#include "Vector.h"
#include "../server/Food.h"

Vector * initVector() {
    Vector * newVector = (Vector *) malloc(sizeof(Vector));

    if (newVector == NULL) {
        perror("Failed to allocate memory to vector.");
        return NULL;
    }
    // No elements are in the vector yet.
    newVector->size = 0;
    newVector->data = NULL;
    return newVector;
}

int addItemToVector(Vector *vector, void *item) {
    // Realloc needs to have previous memory allocated to it to be used.
    if (vector->size == 0) {
        vector->data = (void **) malloc((vector->size + 1)* sizeof(*vector->data));
        if (vector->data == NULL) {
            perror("Failed to allocate memory to Food in vector.");
            return -1;
        }
        // Add properties to the new allocated memory.
        vector->data[0] = item;
        vector->size++;
        return 1;
    }

    void ** temp;
    temp = (void **) realloc(vector->data, (vector->size + 1) * sizeof(*vector->data));

    if (temp == NULL) {
        perror("Failed to re-allocate memory to Food in vector.");
        // Different error since when realloc fails, the previous memory is still there.
        return -2;
    }
    vector->data = temp;
    vector->data[vector->size] = item;
    vector->size++;
    return 1;
}

void deleteVector(Vector * vector) {
    // Free the array.
    free(vector->data);
    // Free the struct.
    free(vector);
}

int deleteItemFromVector(Vector *vector, void *item) {
    int index = -1;
    // Find the index of the pointer to be deleted
    for (int i = 0; i < vector->size; i++) {
        if (item == vector->data[i]) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        return -1;
    }
    // Free the searched item
    free(item);
    // Move memory to the left to delete the selected Food.
    for (int i = index; i < vector->size - 1; i++) {
        vector->data[i] = vector->data[i + 1];
    }
    // Decrement size.
    vector->size--;
    // Size is 0, do not rely on realloc
    if (vector->size == 0) {
        free(vector->data);
    } else {
        // Realloc new size.
        void ** temp = (void **) realloc(vector->data, vector->size * sizeof(*vector->data));
        if (temp == NULL) {
            perror("Failed to de-allocate memory to vector.");
            return -2;
        }
        vector->data = temp;
    }
    return 1;
}
