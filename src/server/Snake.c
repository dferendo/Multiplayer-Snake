//
// Created by dylan on 11/04/2017.
//
#include "Snake.h"
#include "../utility/RandomUtility.h"
#include "../settings/GameSettings.h"
#include "SnakeMove.h"

Snake *createSnake(Vector * connections, Vector * foods) {
    Position * allPositions[DEFAULT_START_SIZE];
    Snake * snake;
    allPositions[0] = createInitialSnakeRandomPosition(connections, foods);

    if (allPositions[0] == NULL) {
        return NULL;
    }

    LinkedListPosition * linkedListPosition = initLinkedListPosition(allPositions[0]);

    if (linkedListPosition == NULL) {
        return NULL;
    }

    snake = (Snake *) malloc(sizeof(Snake));

    if (snake == NULL) {
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
