//
// Created by dylan on 11/04/2017.
//
#include "Snake.h"
#include "GameBoard.h"
#include "CharactersDefinition.h"
#include <stdlib.h>
#include <unistd.h>

void movePositionsOfSnakeToTheLeft(WINDOW * window, LinkedListPosition * currentNode, Position * newPosition) {
    Position * temp;
    // Continue to change positions until the last element is reached.
    if (currentNode->next != NULL) {
        temp = currentNode->position;
        currentNode->position = newPosition;
        mvwaddch(window, currentNode->position->y, currentNode->position->x, SNAKE_BODY);
        movePositionsOfSnakeToTheLeft(window, currentNode->next, temp);
    } else {
        // The last element will have the useless position (old) that needs to be discarded.
        temp = currentNode->position;
        currentNode->position = newPosition;
        mvwaddch(window, currentNode->position->y, currentNode->position->x, SNAKE_BODY);
        free(temp);
    }
}


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
    mvwaddch(window, position->y, position->x, SNAKE_BODY);
    wrefresh(window);
    return snake;
}

int moveSnake(WINDOW * window, Snake * snake) {
    Position * newPosition;
    int x = -1, y = -1;
    // A snake will still be moved even though a button was not pressed, which is
    // determined by the direction the snake is currently facing.
    switch (snake->direction) {
        case D_UP:
            x = snake->positions->position->x;
            y = snake->positions->position->y - 1;
            break;
        case D_LEFT:
            x = snake->positions->position->x - 1;
            y = snake->positions->position->y;
            break;
        case D_DOWN:
            x = snake->positions->position->x;
            y = snake->positions->position->y + 1;
            break;
        case D_RIGHT:
            x = snake->positions->position->x + 1;
            y = snake->positions->position->y;
            break;
    }
    // Clear the window.
    window = resetWindow(window);
    newPosition = malloc(sizeof(Position));
    if (newPosition == NULL) {
        perror("Failed to allocate memory to Position.");
        return -1;
    }
    newPosition->x = x;
    newPosition->y = y;
    // Update all the positions the snake had according to the new Move.
    movePositionsOfSnakeToTheLeft(window, snake->positions, newPosition);
    wrefresh(window);
    return 0;
}

int growSnake(WINDOW *window, Snake *snake) {
    // Allocate 1 more space to the snake.
    Position * position = malloc(sizeof(Position));
    if (position == NULL) {
        perror("Failed to allocate memory to position when snake is growing.");
        return -1;
    }
    // The position is currently don't care and the snake will be seen that it was
    // grown was the next move.
    position->x = -1;
    position->y = -1;
    snake->size++;
    if (addPosition(snake->positions, position) == -1) {
        return -2;
    }
    return 1;
}
