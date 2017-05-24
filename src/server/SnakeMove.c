//
// Created by dylan on 19/04/2017.
//
#include "SnakeMove.h"
#include "Food.h"
#include "../settings/GameSettings.h"
#include "ServerHandle.h"
#include "API/SnakesAPI.h"

SnakeStatus snakeAction(Snake * snake, Vector * foods, Vector * connections) {
    bool error;

    checkIfSnakeHeadPointsToTheNextSnakePosition(snake);

    // Check Head collisions
    if (checkHeadCollision(snake, connections)) {
        return DIED;
    } else if (checkIfNextPositionIsCollision(snake, connections)) {
        return DIED;
    }
    // Check if next position is food and grow if so
    if (checkIfNextPositionIsFoodAndGrow(snake, foods)) {
        // Food was eaten, re-send food data
        do {
            // Send data again, if a connection is lost re-send the data.
            error = writeFoodDataToClients(connections, foods);;
        } while (!error);
    }

    if (snake->size == FOOD_TO_WIN) {
        return WINNER;
    }

    return NORMAL;
}

bool checkHeadCollision(Snake * snake, Vector * connections) {
    Snake * tempSnake;
    Position * nextPositionOfSelectedSnake, * nextPositionOfComparingSnake;
    // Get the next position of this snake
    nextPositionOfSelectedSnake = moveHeadSnake(snake->direction, snake->positions->position);
    // Check if two heads of snake collide, if they do they both die.
    for (int i = 0; i < connections->size; i++) {
        tempSnake = ((Connection *) connections->data[i])->snake;
        // Skips if same snake
        if (tempSnake == snake) {
            continue;
        }
        nextPositionOfComparingSnake = moveHeadSnake(tempSnake->direction,
                                                     tempSnake->positions->position);

        // If same position indicate that this snake should be deleted.
        // Do not remove now since it can cause trouble with the other threads.
        if (nextPositionOfSelectedSnake->y == nextPositionOfComparingSnake->y &&
            nextPositionOfSelectedSnake->x == nextPositionOfComparingSnake->x) {
            free(nextPositionOfComparingSnake);
            free(nextPositionOfSelectedSnake);
            // Indicate that there is a head collision.
            return true;
        }
        // Clear position.
        free(nextPositionOfComparingSnake);
    }
    free(nextPositionOfSelectedSnake);
    return false;
}

Position * moveHeadSnake(Direction direction, Position *position) {
    Position * tempPosition = (Position *) malloc(sizeof(Position));

    if (tempPosition == NULL) {
        perror("Failed to allocate memory to position");
        return NULL;
    }
    tempPosition->x = position->x;
    tempPosition->y = position->y;

    switch (direction) {
        case D_UP:
            tempPosition->y -= 1;
            break;
        case D_LEFT:
            tempPosition->x -= 1;
            break;
        case D_DOWN:
            tempPosition->y += 1;
            break;
        case D_RIGHT:
            tempPosition->x += 1;
            break;
    }
    return tempPosition;
}

bool checkIfNextPositionIsFoodAndGrow(Snake *snake, Vector *foods) {
    Position * nextPositionOfSelectedSnake, * eatenPosition;
    Food * food;
    // Get the next position of this snake
    nextPositionOfSelectedSnake = moveHeadSnake(snake->direction, snake->positions->position);

    // Snakes can move before any foods spawning.
    if (foods != NULL) {
        for (int i = 0; i < foods->size; i++) {
            food = (Food *) foods->data[i];
            // If they are equal, increase the size of snake.
            if (checkIfPositionsAreEqual(nextPositionOfSelectedSnake, food->position)) {
                eatenPosition = (Position *) malloc(sizeof(Position));
                if (eatenPosition == NULL) {
                    perror("Failed to allocate memory to Position");
                    return false;
                }
                // The position does not care since it will handled by move.
                addPosition(snake->positions, eatenPosition);
                snake->size++;
                free(nextPositionOfSelectedSnake);
                // Remove the eaten food.
                free(food->position);
                deleteItemFromVector(foods, food);
                return true;
            }
        }
    }
    free(nextPositionOfSelectedSnake);
    return false;
}

bool checkIfNextPositionIsCollision(Snake *snake, Vector *connections) {
    Position * nextPositionOfSelectedSnake, * collisionPosition;
    Snake * nextSnake;
    bool exists;
    // Get the next position of this snake
    nextPositionOfSelectedSnake = moveHeadSnake(snake->direction, snake->positions->position);

    // The only border is the outside border.
    if (nextPositionOfSelectedSnake->x < 0 || nextPositionOfSelectedSnake->x >= MAIN_WINDOW_COLUMN - 1 ||
        nextPositionOfSelectedSnake->y < 0 ||nextPositionOfSelectedSnake->y >= MAIN_WINDOW_ROW - 1) {
        free(nextPositionOfSelectedSnake);
        return true;
    }
    // Check if collision with other snake
    for (int i = 0; i < connections->size; i++) {
        exists = false;
        nextSnake = ((Connection *) connections->data[i])->snake;
        positionExistsLinkedList(nextSnake->positions, nextPositionOfSelectedSnake->x,
                                 nextPositionOfSelectedSnake->y, &exists);
        // If true, there is a collision.
        if (exists) {
            // Check if the collision happened to the last element of the snake.
            collisionPosition = getLastElementLinkedList(nextSnake->positions);
            // Since it is the last element, the other snake will also move
            // and thus no collision.
            if (collisionPosition->y == nextPositionOfSelectedSnake->y &&
                collisionPosition->x == nextPositionOfSelectedSnake->x) {
                free(nextPositionOfSelectedSnake);
                return false;
            }
            free(nextPositionOfSelectedSnake);
            return true;
        }
    }
    free(nextPositionOfSelectedSnake);
    return false;
}

void snakeMove(Snake *snake) {
    LinkedListPosition * currentHead = snake->positions->next;
    Position * previousPosition = snake->positions->position, * currentPosition,
            * lastElement, * nextPositionOfSelectedSnake;

    nextPositionOfSelectedSnake = moveHeadSnake(snake->direction, snake->positions->position);

    // Last element is useless and thus freed.
    lastElement = getLastElementLinkedList(snake->positions);
    free (lastElement);

    while (currentHead != NULL) {
        currentPosition = currentHead->position;
        currentHead->position = previousPosition;
        currentHead = currentHead->next;
        previousPosition = currentPosition;
    }
    // The head still needs to be updated.
    snake->positions->position = nextPositionOfSelectedSnake;
}

void checkIfSnakeHeadPointsToTheNextSnakePosition(Snake *snake) {
    Position * nextPositionOfSelectedSnake;

    nextPositionOfSelectedSnake = moveHeadSnake(snake->direction, snake->positions->position);

    if (snake->positions->next->position->x == nextPositionOfSelectedSnake->x &&
            snake->positions->next->position->y == nextPositionOfSelectedSnake->y) {
        // Change direction since the user tried to kill himself by pressing too
        // many direction resulting the head going to the snake 2nd position
        snake->direction = OPPOSITE_DIR(snake->direction);
    }

    free(nextPositionOfSelectedSnake);
}
