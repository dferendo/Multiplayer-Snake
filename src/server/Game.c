//
// Created by dylan on 18/04/2017.
//
#include <pthread.h>
#include "Game.h"
#include "Server.h"
#include "Food.h"

Vector * connections;
Vector * foods;
pthread_mutex_t lock;

void gameInitialize() {
    pthread_t foodThread;
    // Where all the food will be placed.
    food = initVector();
    // Create thread for food.
    if (pthread_create(&foodThread, NULL, generateFood, NULL) != 0) {
        perror("Could not create a food thread.");
        return;
    }

    while (true) {
        
    }
}
