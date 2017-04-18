//
// Created by dylan on 18/04/2017.
//
#include "Game.h"
#include "Server.h"

Vector * connections;
Vector * foods;
pthread_mutex_t lock;

void gameInitialize() {
    // Where all the food will be placed.
    food = initVector();
    // Create thread for food.
}
