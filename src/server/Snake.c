//
// Created by dylan on 11/04/2017.
//
#include "Snake.h"
#include "../utility/RandomUtility.h"
#include "../template/GameSettings.h"

Snake *createSnake(Vector *initialPositions) {
    Position * position = createInitialSnakeRandomPosition(initialPositions);
    LinkedListPosition * linkedListPosition = initLinkedListPosition(position);

    Snake * snake = (Snake *) malloc(sizeof(Snake));

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
