//
// Created by dylan on 17/04/2017.
//
#include "SnakesGame.h"
#include "../server/Server.h"
#include <stdio.h>

void gameInit(Vector * connections) {
    if (connections->size == 0) {
        perror("There are not connections.");
        return;
    }
}

void sendAllSnakeDataToAllConnections(Vector *connections) {
//    size_t size = connections->size *
}
