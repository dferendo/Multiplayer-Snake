//
// Created by dylan on 11/04/2017.
//

#ifndef SNAKES_LINKEDLIST_H
#define SNAKES_LINKEDLIST_H

#include <stdbool.h>

typedef struct Position Position;

typedef struct LinkedListPosition {
    Position * position;
    struct LinkedListPosition * next;
} LinkedListPosition;

/**
 * Initialise a linked list containing {@link Position} items.
 *
 * @param position: The first position of the linked list.
 * @return: The head of the linked list, NULL if an error has occured.
 */
LinkedListPosition * initLinkedListPosition(Position * position);

/**
 * Add a position to the given linked list.
 *
 * @param head: The head of the linked list.
 * @param position: The new position to be added.
 * @return: 1 if the item was added, -1 if there was an error.
 */
int addPosition(LinkedListPosition * head, Position * position);

/**
 * Delete the linked list and also frees the position.
 *
 * @param head: The head of the linked list.
 */
void deleteLinkedListPosition(LinkedListPosition * head);

/**
 * Checks if the given x, y coordinates corresponds to a position found in the linked
 * list.
 *
 * @param head: The head of the linked list.
 * @param x: The x-coordinate.
 * @param y: The y-coordinate.
 * @param exist: Will change to true if position is the same, remains the same otherwise.
 */
void positionExistsLinkedList(LinkedListPosition *head, int x, int y, bool *exist);

/**
 * Get the last position of the given linked list.
 *
 * @param head: The head of the linked list.
 * @return The last element of the passed linked list.
 */
Position * getLastElementLinkedList(LinkedListPosition * head);

#endif //SNAKES_LINKEDLIST_H
