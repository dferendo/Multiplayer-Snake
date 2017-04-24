//
// Created by dylan on 12/04/2017.
//

#ifndef SNAKES_VECTOR_H
#define SNAKES_VECTOR_H

#include <stdlib.h>

/**
 * Dynamic allocated array.
 * data: Holds an array of pointers of type (void *).
 * size: The current amount of elements in the vector.
 */
typedef struct Vector {
    void ** data;
    size_t size;
} Vector;

/**
 * Allocate memory for the struct of the Vector. Warning no elements are yet in the vector.
 *
 * @return: Pointer to a Vector, NULL if an error occurred.
 */
Vector * initVector();

/**
 * Re-alloc data of the Vector to hold the new element passed as a parameter.
 *
 * @param vector: The Vector to put the item in.
 * @param item: The new item.
 * @return 1 if the item was added to the Vector.
 *         -1 if the malloc when there was no items in the vector failed.
 *         -2 if re-alloc failed.
 */
int addItemToVector(Vector *vector, void *item);

/**
 * Free all memory used by the Vector.
 *
 * @param vector: Vector containing all elements.
 *
 * @warning: no pointers of the data are freed.
 */
void deleteVector(Vector * vector);

/**
 * Deletes an element in the Vector and re-size the vector accordingly.
 *
 * @param vector: Vector containing all elements.
 * @param item: The pointer to the item that needs to be deleted.
 * @return: -1 if the item is not found in the Vector.
 *          -2 if re-alloc failed.
 *          1 if the item was deleted successfully.
 */
int deleteItemFromVector(Vector *vector, void *item);

#endif //SNAKES_VECTOR_H
