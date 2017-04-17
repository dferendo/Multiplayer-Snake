//
// Created by dylan on 11/04/2017.
//
#include <stdlib.h>
#include "LinkedList.h"
#include "../server/Snake.h"

LinkedListPosition * initLinkedListPosition(Position *position) {
    LinkedListPosition *linkedList = (LinkedListPosition *) malloc(sizeof(LinkedListPosition));

    if (linkedList == NULL) {
        perror("Failed to allocate memory to linked list.");
        return NULL;
    }

    linkedList->position = position;
    linkedList->next = NULL;
    return linkedList;
}

void deleteLinkedListPosition(LinkedListPosition *head) {
    if (head->next != NULL) {
        deleteLinkedListPosition(head->next);
    }
    free(head->position);
    free(head);
}

int addPosition(LinkedListPosition *head, Position *position) {
    while (head->next != NULL) {
        head = head->next;
    }
    LinkedListPosition *newEntry = initLinkedListPosition(position);

    if (newEntry == NULL) {
        perror("Failed to add to the LinkedList.");
        return -1;
    }
    head->next = newEntry;
    return 1;
}
