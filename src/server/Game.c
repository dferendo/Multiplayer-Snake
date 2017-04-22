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
#include <unistd.h>

pthread_mutex_t lock;

void * gameInitialize(void * args) {
    Vector * connections = ((GameThreadParams *) args)->connections;
    Vector * foods = ((GameThreadParams *) args)->foods;
    pthread_t foodThread, changeDirectionThread;

    ChangeDirectionParams * changeDirectionParams =
            (ChangeDirectionParams *) malloc(sizeof(ChangeDirectionParams));

    if (changeDirectionParams == NULL) {
        perror("Cannot allocate memory to Params");
        free(args);
        pthread_exit(NULL);
    }
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

    // Create thread for food.
    if (pthread_create(&foodThread, NULL, generateFood, foodGeneratorParams) != 0) {
        perror("Could not create a food thread");
        free(args);
        free(changeDirectionParams);
        free(foodGeneratorParams);
        pthread_exit(NULL);
    }
    // Create Thread that listens to user change of direction.
    if (pthread_create(&changeDirectionThread, NULL, checkForChangeOfDirections,
                       changeDirectionParams) != 0) {
        perror("Could not create a Change direction thread");
        free(args);
        free(changeDirectionParams);
        free(foodGeneratorParams);
        pthread_exit(NULL);
    }
    gameLoop(connections, foods, lock);
    free(args);
    pthread_exit(NULL);
}

void gameLoop(Vector *connections, Vector *foods, pthread_mutex_t lock) {

    while (true) {
        // Lock so that food is not generated when finding the new location.
        pthread_mutex_lock(&lock);
        if (connections != NULL) {
            // Move snakes
            moveSnakes(connections, foods);
            // Send new information.
            sendSnakeDataToClients(connections);
        }
        pthread_mutex_unlock(&lock);
        usleep(GAME_UPDATE_RATE_US);
    }
}

bool moveSnakes(Vector *connections, Vector *foods) {
    Snake * snake;
    SnakeStatus moveSnakesReturns[connections->size];
    bool thereAreWinners = false;
    Connection * connection;

    // Move snakes
    for (int i = 0; i < connections->size; i++) {
        snake = ((Connection *) connections->data[i])->snake;
        moveSnakesReturns[i] = snakeAction(snake, foods, connections);
        if (moveSnakesReturns[i] == WINNER) {
            thereAreWinners = true;
        }
    }
    // If there is a winner tell non-winners they lost.
    // Multiple winners are possible.
    if (thereAreWinners) {
        for (int i = 0; i < connections->size; i++) {
            connection = (Connection *)connections->data[i];
            if (moveSnakesReturns[i] == WINNER) {
                sendEndGameToClients(connection->sockFd, WINNER);
            } else {
                sendEndGameToClients(connection->sockFd, DIED);
            }
        }
    }

    // Check if snakes died.
    for (int i = 0; i < connections->size; i++) {
        connection = (Connection *)connections->data[i];
        if (moveSnakesReturns[i] == DIED) {
            sendEndGameToClients(connection->sockFd, DIED);
            // Clear snake.
            freeConnection(connection);
            deleteItemFromVector(connections, connection);
        }
    }
    return thereAreWinners;
}