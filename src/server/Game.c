//
// Created by dylan on 18/04/2017.
//
#include <pthread.h>
#include "Server.h"
#include "Game.h"
#include "SnakeMove.h"
#include "Food.h"
#include "../utility/Serialize.h"
#include <unistd.h>
#include <strings.h>

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

    gameLoop();
}

void gameLoop() {

    while (true) {
        usleep(GAME_UPDATE_RATE_US);
        // Lock so that food is not generated when finding the new location.
        pthread_mutex_lock(&lock);
        // Create thread workers.
        createSnakeWorkers();
        // Send new information.
        sendSnakeInformationToClients();
        pthread_mutex_unlock(&lock);
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
        snakeWorkerReturn[i].status = ((SnakeWorkerReturn *) threadReturn)->status;
    }

}