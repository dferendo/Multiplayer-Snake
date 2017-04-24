//
// Created by dylan on 11/04/2017.
//
#include "Snake.h"
#include "../utility/RandomUtility.h"
#include "../settings/GameSettings.h"

Snake *createSnake(Vector * connections, Vector * foods, bool restart, int currentCount) {
    Position * allPositions[DEFAULT_START_SIZE], * position;
    Snake * snake;
    LinkedListPosition * linkedListPosition;

    if (restart) {
        allPositions[0] = createInitialSnakeRandomPositionForRestart(connections, foods, currentCount);
    } else {
        allPositions[0] = createInitialSnakeRandomPosition(connections, foods);
    }

    if (allPositions[0] == NULL) {
        return NULL;
    }

    // Add the default snakes positions, these positions were checked by the first positions
    // to see that they were not taken
    for (int i = 1; i < DEFAULT_START_SIZE; i++) {
        position = (Position *) malloc(sizeof(Position));

        if (position == NULL) {
            // Remove other allocated positions
            for (int j = 0; j < i; j++) {
                free(allPositions[j]);
            }
            return NULL;
        }
        position->x = allPositions[0]->x + i;
        position->y = allPositions[0]->y;
        allPositions[i] = position;
    }

    linkedListPosition = initLinkedListPosition(allPositions[0]);

    if (linkedListPosition == NULL) {
        for (int i = 0; i < DEFAULT_START_SIZE; i++) {
            free(allPositions[i]);
        }
        return NULL;
    }
    // Add items to linked list
    for (int i = 1; i < DEFAULT_START_SIZE; i++) {
        addPosition(linkedListPosition, allPositions[i]);
    }

    snake = (Snake *) malloc(sizeof(Snake));

    if (snake == NULL) {
        deleteLinkedListPosition(linkedListPosition);
        perror("Failed to allocate memory to Snake");
        return NULL;
    }

    snake->size = DEFAULT_START_SIZE;
    snake->positions = linkedListPosition;
    snake->direction = DEFAULT_START_DIRECTION;
    return snake;
}

bool checkIfPositionsAreEqual(Position *position1, Position *position2) {
    if (position1->x == position2->x && position1->y == position2->y) {
        return true;
    }
    return false;
}
