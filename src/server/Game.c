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
Vector * foods;
Vector * connections;

void * gameInitialize(void * args) {
    pthread_t foodThread, changeDirectionThread;
    bool * keepChangeDirectionThread = (bool *) malloc(sizeof(bool)),
            * keepFoodGeneratorThread = (bool *) malloc(sizeof(bool));

    if (keepChangeDirectionThread == NULL) {
        perror("Failed to allocate memory to Params");
        pthread_exit(NULL);
    } else if (keepFoodGeneratorThread == NULL) {
        free(keepChangeDirectionThread);
        perror("Failed to allocate memory to Params");
        pthread_exit(NULL);
    }
    *keepChangeDirectionThread = true;
    *keepFoodGeneratorThread = true;

    ChangeDirectionParams * changeDirectionParams =
            (ChangeDirectionParams *) malloc(sizeof(ChangeDirectionParams));

    if (changeDirectionParams == NULL) {
        perror("Cannot allocate memory to Params");
        pthread_exit(NULL);
    }
    changeDirectionParams->connections = connections;
    changeDirectionParams->killThread = keepChangeDirectionThread;

    FoodGeneratorParams * foodGeneratorParams =
            (FoodGeneratorParams *) malloc(sizeof(FoodGeneratorParams));

    if (foodGeneratorParams == NULL) {
        perror("Cannot allocate memory to Params");
        free(changeDirectionParams);
        pthread_exit(NULL);
    }
    foodGeneratorParams->foods = foods;
    foodGeneratorParams->connections = connections;
    foodGeneratorParams->killThread = keepFoodGeneratorThread;

    // Create thread for food.
    if (pthread_create(&foodThread, NULL, generateFood, foodGeneratorParams) != 0) {
        perror("Could not create a food thread");
        free(changeDirectionParams);
        free(foodGeneratorParams);
        pthread_exit(NULL);
    }
    // Create Thread that listens to user change of direction.
    if (pthread_create(&changeDirectionThread, NULL, checkForChangeOfDirections,
                       changeDirectionParams) != 0) {
        perror("Could not create a Change direction thread");
        free(changeDirectionParams);
        free(foodGeneratorParams);
        pthread_exit(NULL);
    }

    gameLoop(connections, foods);
    // Kill threads
    *keepChangeDirectionThread = false;
    *keepFoodGeneratorThread = false;
    // Wait for thread to finish
    pthread_join(changeDirectionThread, NULL);
    pthread_join(foodThread, NULL);
    // Clear data used except connections
    pthread_mutex_lock(&lock);
    clearDataUsedForGame(connections, foods);
    foods = initVector();
    pthread_mutex_unlock(&lock);
    // Exit
    pthread_exit(NULL);
}

void gameLoop(Vector *connections, Vector *foods) {
    bool error;

    while (true) {
        // Lock so that food is not generated when finding the new location.
        pthread_mutex_lock(&lock);
        if (connections != NULL) {
            // Move snakes, if true there is a winner and game needs to be restarted.
            if (moveSnakes(connections, foods)) {
                pthread_mutex_unlock(&lock);
                break;
            }
            // Send new information.
            do {
                // Send data again, if a connection is lost re-send the data.
                error = sendSnakeDataToClients(connections);
            } while (!error);
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
                // Check for connection lost, if so remove the connection
                if (!sendEndGameToClients(connection->sockFd, WINNER)) {
                    freeConnection(connection);
                    deleteItemFromVector(connections, connection);
                }
            } else {
                // Check for connection lost, if so remove the connection
                if (!sendEndGameToClients(connection->sockFd, RESTART)) {
                    freeConnection(connection);
                    deleteItemFromVector(connections, connection);
                }
            }
        }
        return true;
    } else {
        // Check if snakes died.
        for (int i = 0; i < connections->size; i++) {
            connection = (Connection *) connections->data[i];
            if (moveSnakesReturns[i] == DIED) {
                sendEndGameToClients(connection->sockFd, DIED);
                // Clear snake, regardless if connection failed
                freeConnection(connection);
                deleteItemFromVector(connections, connection);
            }
        }
    }
    return false;
}

void clearDataUsedForGame(Vector *connections, Vector *foods) {
    Connection * connection;
    // Clear food
    clearFoodsVector(foods);
    // Clear snakes
    for (int i = 0; i < connections->size; i++) {
        connection = (Connection *) connections->data[i];
        freeSnake(connection->snake);
    }
}
