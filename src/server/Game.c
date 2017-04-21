//
// Created by dylan on 18/04/2017.
//
#include <pthread.h>
#include "Server.h"
#include "Game.h"
#include "SnakeMove.h"
#include "../utility/General.h"
#include "Food.h"
#include "../settings/GameSettings.h"
#include "API/SnakesAPI.h"
#include "ServerHandle.h"
#include <unistd.h>

void * gameInitialize(void * args) {
    Vector * connections = ((GameThreadParams *) args)->connections;
    Vector * foods = ((GameThreadParams *) args)->foods;
    pthread_mutex_t lock = ((GameThreadParams *) args)->lock;
    pthread_t foodThread, changeDirectionThread;

    ChangeDirectionParams * changeDirectionParams =
            (ChangeDirectionParams *) malloc(sizeof(ChangeDirectionParams));

    if (changeDirectionParams == NULL) {
        perror("Cannot allocate memory to Params");
        free(args);
        pthread_exit(NULL);
    }
    changeDirectionParams->lock = lock;
    changeDirectionParams->connections = connections;

    FoodGeneratorParams * foodGeneratorParams =
            (FoodGeneratorParams *) malloc(sizeof(FoodGeneratorParams));

    if (foodGeneratorParams == NULL) {
        perror("Cannot allocate memory to Params");
        free(args);
        free(changeDirectionParams);
        pthread_exit(NULL);
    }
    foodGeneratorParams->foods = foods;
    foodGeneratorParams->connections = connections;
    foodGeneratorParams->lock = lock;

    // Create thread for food.
    if (pthread_create(&foodThread, NULL, generateFood, foodGeneratorParams) != 0) {
        perror("Could not create a food thread");
        free(args);
        free(changeDirectionParams);
        free(foodGeneratorParams);
        pthread_exit(NULL);
    }
    // Create Thread that listens to user change of direction.
//    if (pthread_create(&changeDirectionThread, NULL, checkForChangeOfDirections,
//                       changeDirectionParams) != 0) {
//        perror("Could not create a Change direction thread");
//        free(args);
//        free(changeDirectionParams);
//        free(foodGeneratorParams);
//        pthread_exit(NULL);
//    }
//    gameLoop(connections, foods, lock);
    free(args);
    pthread_exit(NULL);
}

void gameLoop(Vector *connections, Vector *foods, pthread_mutex_t lock) {

    while (true) {
        // Lock so that food is not generated when finding the new location.
        pthread_mutex_lock(&lock);
        // Create thread workers.
        if (createSnakeWorkers(connections, foods)) {
            break;
        }
        // Send new information.
        sendSnakeDataToClients(connections);
        pthread_mutex_unlock(&lock);
        usleep(GAME_UPDATE_RATE_US);
    }
}

bool createSnakeWorkers(Vector *connections, Vector *foods) {
    Snake * snake;
    bool thereAreWinners = false;
    pthread_t  snakesTIds[connections->size];
    Connection * connection;
    // Create a thread for every Snake.
    SnakeWorkerParams snakeWorkerParams[connections->size];
    SnakeWorkerReturn snakeWorkerReturn[connections->size];

    for (int i = 0; i < connections->size; i++) {
        snake = ((Connection *) connections->data[i])->snake;
        // Set params to be passed to pthread
        snakeWorkerParams[i].connections = connections;
        snakeWorkerParams[i].foods = foods;
        snakeWorkerParams[i].snake = snake;
        // Create Threads
        // Every thread can modify the snake since they are different.
        if (pthread_create(&snakesTIds[i], NULL, snakeAction, snakeWorkerParams + i) != 0) {
            perror("Could not create a snake thread.");
            pthread_exit(NULL);
        }
    }
    // Wait for threads
    for (int i = 0; i < connections->size; i++) {
        // Method returns nothing.
        void * threadReturn;
        pthread_join(snakesTIds[i], &threadReturn);
        snakeWorkerReturn[i].status = ((SnakeWorkerReturn *) threadReturn)->status;
        // Check for winners.
        if (snakeWorkerReturn[i].status == WINNER) {
            thereAreWinners = true;
        }
    }
    // If there is a winner tell non-winners they lost.
    // Multiple winners are possible.
    if (thereAreWinners) {
        for (int i = 0; i < connections->size; i++) {
            connection = (Connection *)connections->data[i];
            if (snakeWorkerReturn[i].status == WINNER) {
                sendEndGameToClients(connection->sockFd, WINNER);
            } else {
                sendEndGameToClients(connection->sockFd, DIED);
            }
        }
    }
    // Check if snakes died.
    for (int i = 0; i < connections->size; i++) {
        connection = (Connection *)connections->data[i];
        if (snakeWorkerReturn[i].status == DIED) {
            sendEndGameToClients(connection->sockFd, DIED);
            // Clear snake.
            freeConnection(connection);
            deleteItemFromVector(connections, connection);
        }
    }
    // If no connections, stop game.
    return thereAreWinners;
}
