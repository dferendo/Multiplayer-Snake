//
// Created by dylan on 12/04/2017.
//

#include <stdlib.h>
#include <memory.h>
#include "Vector.h"
#include "../snakes-components/Food.h"

vectorFood * initModifiedVectorFood() {
    vectorFood * newVector = (vectorFood *) malloc(sizeof(vectorFood));

    if (newVector == NULL) {
        perror("Failed to allocate memory to vector.");
        return NULL;
    }
    // No elements are in the vector yet.
    newVector->size = 0;
    return newVector;
}

int addFoodToModifiedVector(vectorFood * vector, Food *food) {
    // Realloc needs to have previous memory allocated to it to be used.
    if (vector->size == 0) {
        vector->size++;
        vector->foods = (Food **) malloc(vector->size * sizeof(Food));
        if (vector->foods == NULL) {
            perror("Failed to allocate memory to Food in vector.");
            return -1;
        }
        // Add properties to the new allocated memory.
        vector->foods[0] = food;
        return 1;
    }

    vector->size++;
    Food ** temp;
    temp = (Food **) realloc(vector->foods, vector->size * sizeof(Food));

    if (temp == NULL) {
        perror("Failed to re-allocate memory to Food in vector.");
        // Different error since when realloc fails, the previous memory is still there.
        return -2;
    }
    vector->foods = temp;
    vector->foods[vector->size - 1] = food;
    return 1;
}

void deleteVector(vectorFood *vector) {
    // Free the array of pointers to food.
    for (int i = 0; i < vector->size; i++) {
        free(vector->foods[i]->position);
        free(vector->foods[i]);
    }
    // Free the array.
    free(vector->foods);
    // Free the struct.
    free(vector);
}

int deleteItem(vectorFood * vector, Food * food) {
    int index = -1;
    // Find the index of the food that will be deleted.
    for (int i = 0; i < vector->size; i++) {
        if (foodEquals(food, vector->foods[i])) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        // Food not found.
        return -1;
    }
    // Free the deleted Food.
    free(food->position);
    free(food);
    // TODO memmove use.
    // Move memory to the left to delete the selected Food.
    for (int i = index; i < vector->size - 1; i++) {
        vector->foods[i] = vector->foods[i + 1];
    }
    // Decrement size.
    vector->size--;
    // Realloc new size.
    Food ** temp = (Food **) realloc(vector->foods, vector->size * sizeof(Food));
    if (temp == NULL) {
        perror("Failed to de-allocate memory to Food vector.");
        return -2;
    }
    vector->foods = temp;
    return 1;
}
