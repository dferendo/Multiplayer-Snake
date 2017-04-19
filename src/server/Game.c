//
// Created by dylan on 18/04/2017.
//
#include <pthread.h>
#include <strings.h>
#include "Server.h"
#include "Game.h"
#include "Food.h"
#include "../utility/Serialize.h"
#include "../template/GameSettings.h"
#include "Snake.h"
#include <unistd.h>

Vector * connections;
Vector * foods;
pthread_mutex_t lock;

void gameInitialize() {
    pthread_t foodThread;
    // Where all the food will be placed.
    foods = initVector();
    // Send initial snake information
    sendSnakeInformationToClients();
    // Create thread for food.
    if (pthread_create(&foodThread, NULL, generateFood, NULL) != 0) {
        perror("Could not create a food thread");
        return;
    }

    while (true) {
        usleep(GAME_UPDATE_RATE_US);
        // Create threads and wait for them to finish.
        createSnakeWorkers();
        // Send new information.
        sendSnakeInformationToClients();
    }
}

void sendSnakeInformationToClients() {
    int response;
    // Size without Positions of snake.
    // The information consists of a delimiter and for each snake, it contains the
    // owner name and the size of the snake and the current direction.
    size_t size = DELIMITERS_SIZE + (connections->size * MAXIMUM_INPUT_STRING) +
                  (connections->size * (INTEGER_BYTES * 2));

    // Calculate the size of positions of the snake.
    for (int i = 0; i < connections->size; i++) {
        size += (((Connection *) connections->data[i])->clientInfo->snake->size) *
                POSITION_BYTES;
    }

    unsigned char buffer[size];
    bzero(buffer, size);

    serializedSnakeFromConnections(buffer, connections);

    // Write to all the clients about the information.
    for (int i = 0; i < connections->size; i++) {
        struct Connection * connection = (Connection *) connections->data[i];

        response = (int) write(connection->sockFd, buffer, size);

        if (response == -1) {
            perror("Failed to write to the socket");
            close(connection->sockFd);
        }
    }
}

void createSnakeWorkers() {
    Snake * snake;
    pthread_t  snakesTIds[connections->size];
    // Create a thread for every Snake.
    SnakeWorkerParams snakeWorkerParams[connections->size];
    SnakeWorkerReturn snakeWorkerReturn[connections->size];

    for (int i = 0; i < connections->size; i++) {
        snake = ((Connection *) connections->data[i])->clientInfo->snake;
        // Set params to be passed to pthread
        snakeWorkerParams[i].connections = connections;
        snakeWorkerParams[i].foods = foods;
        snakeWorkerParams[i].snake = snake;
        // Create Threads
        // Every thread can modify the snake since they are different.
        if (pthread_create(&snakesTIds[i], NULL, snakeAction, snakeWorkerParams + i) != 0) {
            perror("Could not create a snake thread.");
            return;
        }
    }
    // Wait for threads
    for (int i = 0; i < connections->size; i++) {
        // Method returns nothing.
        void * threadReturn;
        pthread_join(snakesTIds[i], &threadReturn);
    }
}

void *snakeAction(void *args) {
    Snake * snake = ((SnakeWorkerParams *) args)->snake;
    Vector * foods = ((SnakeWorkerParams *) args)->foods;
    Vector * allConnections = ((SnakeWorkerParams *) args)->connections;
    // For return purposes
    SnakeWorkerReturn * status = (SnakeWorkerReturn *) malloc(sizeof(SnakeWorkerReturn));

    // Check Head collisions
    if (checkHeadCollision(snake, allConnections)) {
        status->status = DIED;
        return (void *) status;
    } else if (checkIfNextPositionIsCollision(snake, allConnections)) {
        status->status = DIED;
        return (void *) status;
    }
    // Check if next position is food and grow if so
    checkIfNextPositionIsFoodAndGrow(snake, foods);

    if (snake->size == FOOD_TO_WIN) {
        status->status = WINNER;
        return (void *) status;
    }
    // Move
    snakeMove(snake);

    // Snake just moved
    status->status = NORMAL;
    return (void *) status;
}

bool checkHeadCollision(Snake * snake, Vector * connections) {
    Snake * tempSnake;
    Position * nextPositionOfSelectedSnake, * nextPositionOfComparingSnake;
    // Get the next position of this snake
    nextPositionOfSelectedSnake = moveHeadSnake(snake->direction, snake->positions->position);
    // Check if two heads of snake collide, if they do they both die.
    for (int i = 0; i < connections->size; i++) {
        tempSnake = ((Connection *) connections->data[i])->clientInfo->snake;
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
            tempPosition->x += 1;
            break;
        case D_DOWN:
            tempPosition->y += 1;
            break;
        case D_RIGHT:
            tempPosition->x -= 1;
            break;
    }
    return tempPosition;
}

void checkIfNextPositionIsFoodAndGrow(Snake *snake, Vector *foods) {
    Position * nextPositionOfSelectedSnake, * eatenPosition;
    Food * food;
    // Get the next position of this snake
    nextPositionOfSelectedSnake = moveHeadSnake(snake->direction, snake->positions->position);

    // Snakes can move before any foods spawning.
    if (foods != NULL) {
        for (int i = 0; i < foods->size; i++) {
            food = (Food *) foods->data;
            // If they are equal, increase the size of snake.
            if (checkIfPositionsAreEqual(nextPositionOfSelectedSnake, food->position)) {
                eatenPosition = (Position *) malloc(sizeof(Position));

                if (eatenPosition == NULL) {
                    perror("Failed to allocate memory to Position");
                    return;
                }
                // The position does not care since it will handled by move.
                addPosition(snake->positions, eatenPosition);
                snake->size++;
                free(nextPositionOfSelectedSnake);
                break;
            }
        }
    }
    free(nextPositionOfSelectedSnake);
}

bool checkIfNextPositionIsCollision(Snake *snake, Vector *connections) {
    Position * nextPositionOfSelectedSnake, * collisionPosition;
    Snake * nextSnake;
    bool exists;
    // Get the next position of this snake
    nextPositionOfSelectedSnake = moveHeadSnake(snake->direction, snake->positions->position);

    // The only border is the outside border.
    if (nextPositionOfSelectedSnake->x < 0 || nextPositionOfSelectedSnake->x >= MAIN_WINDOW_COLUMN ||
        nextPositionOfSelectedSnake->y < 0 ||nextPositionOfSelectedSnake->y >= MAIN_WINDOW_ROW) {
        free(nextPositionOfSelectedSnake);
        return true;
    }
    // Check if collision with other snake
    for (int i = 0; i < connections->size; i++) {
        exists = false;
        nextSnake = ((Connection *) connections->data[i])->clientInfo->snake;
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
