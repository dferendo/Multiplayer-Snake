//
// Created by dylan on 11/04/2017.
//
#include "Snake.h"
#include <stdlib.h>
#include "GameBoard.h"

Snake * createSnake(WINDOW * window) {
    LinkedListPosition * head;
    // TODO: position random.
    Position * position = malloc(sizeof(Position));

    if (position == NULL) {
        perror("Failed to allocate memory for position.");
        return NULL;
    }
    position->x = 12;
    position->y = 12;
    // Malloc a new Snake.
    Snake * snake = malloc(sizeof(Snake));

    if (snake == NULL) {
        perror("Failed to allocate memory for Snake.");
        return NULL;
    }
    snake->direction = DEFAULT_START_DIRECTION;
    snake->size = DEFAULT_START_SIZE;
    // Snake must at least have 1 position.
    head = initLinkedListPosition(position);
    snake->positions = head;
    mvwaddch(window, position->y, position->x, '#');
    wrefresh(window);
    return snake;
}

int moveSnake(WINDOW * window, Snake * snake) {

    switch (snake->direction) {
        case D_UP:
            snake->positions->position->y--;
            break;
        case D_LEFT:
            snake->positions->position->x--;
            break;
        case D_DOWN:
            snake->positions->position->y++;
            break;
        case D_RIGHT:
            snake->positions->position->x++;
            break;
    }
    window = resetWindow(window);
    mvwaddch(window, snake->positions->y, snake->positions->x, '#');
    wrefresh(window);
    return 0;
}

void movePositionsOfSnakeToTheLeft(WINDOW * window, LinkedListPosition * currentNode, Position * newPosition) {
    Position * temp;
    // Continue to change positions until the last element is reached.
    if (currentNode->next != NULL) {
        temp = currentNode->position;
        currentNode->position = newPosition;
        mvwaddch(window, currentNode->position->y, currentNode->position->x, '#');
        movePositionsOfSnakeToTheLeft(window, currentNode->next, temp);
    }
    // The last element will have the useless position (old) that needs to be discarded.
    temp = currentNode->position;
    currentNode->position = newPosition;
    free(temp);
}
