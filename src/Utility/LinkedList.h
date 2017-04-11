//
// Created by dylan on 11/04/2017.
//

#ifndef SNAKES_LINKEDLIST_H
#define SNAKES_LINKEDLIST_H

#include "../snakes-components/Snake.h"

typedef struct LinkedListPosition {
    Position * position;
    struct LinkedListPosition * next;
} LinkedListPosition;

LinkedListPosition * initLinkedListPosition(Position * position);

void deleteLinkedListPosition(LinkedListPosition * head);

int addPosition(LinkedListPosition * head, Position * position);

#endif //SNAKES_LINKEDLIST_H
