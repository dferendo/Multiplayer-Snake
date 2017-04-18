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

LinkedListPosition * initLinkedListPosition(Position * position);

int addPosition(LinkedListPosition * head, Position * position);

void deleteLinkedListPosition(LinkedListPosition * head);

void positionExistsLinkedList(LinkedListPosition *head, int x, int y, bool *exist);

#endif //SNAKES_LINKEDLIST_H
