//
// Created by dylan on 15/04/2017.
//
#include "General.h"
#include "LinkedList.h"
#include "../server/ServerHandle.h"
#include "../server/Food.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

bool setSocketBlockingEnabled(int sockFd, bool blocking) {
    int flags;

    if (sockFd < 0) {
        return false;
    }
    flags = fcntl(sockFd, F_GETFL, 0);
    if (flags < 0) {
        return false;
    }
    flags = blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
    return (fcntl(sockFd, F_SETFL, flags) == 0) ? true : false;
}

void freeDataOfConnection(Connection *connection) {
    // Delete linked list with all the positions
    deleteLinkedListPosition(connection->snake->positions);
    // Free Snake
    free(connection->snake);
    // Close socket
    close(connection->sockFd);
}

void freeSnake(Snake * snake) {
    // Delete linked list with all the positions
    deleteLinkedListPosition(snake->positions);
    snake->size = 0;
    // Free Snake
    free(snake);
}

void clearFoodsVector(Vector *foods) {
    Food * food;
    if (foods == NULL) {
        return;
    }

    for (int i = 0; i < foods->size; i++) {
        food = (Food *) foods->data[i];
        free(food->position);
        free(food);
    }
    deleteVector(foods);
}

void clearSnakeVector(Vector *snakes) {
    Snake * snake;

    if (snakes == NULL) {
        return;
    }

    for (int i = 0; i < snakes->size; i++) {
        snake = (Snake *) snakes->data[i];
        freeSnake(snake);
    }
    deleteVector(snakes);
}