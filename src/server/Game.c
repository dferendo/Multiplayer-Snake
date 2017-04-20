//
// Created by dylan on 18/04/2017.
//
#include <pthread.h>
#include "Server.h"
#include "Game.h"
#include "SnakeMove.h"
#include "../utility/Serialize.h"
#include "../utility/General.h"
#include "Food.h"
#include <unistd.h>
#include <strings.h>
#include <memory.h>

Vector * connections;
Vector * foods;
pthread_mutex_t lock;

void gameInitialize() {
    pthread_t foodThread;
    pthread_t changeDirectionThread;
    // Where all the food will be placed.
    foods = initVector();
    // Send initial snake information
    sendSnakeInformationToClients();
    // Create thread for food.
    if (pthread_create(&foodThread, NULL, generateFood, NULL) != 0) {
        perror("Could not create a food thread");
        return;
    }
    // Create Thread that listens to user change of direction.
    if (pthread_create(&changeDirectionThread, NULL, checkForChangeOfDirections, NULL) != 0) {
        perror("Could not create a Change direction thread");
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
        if (createSnakeWorkers()) {
            break;
        }
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

bool createSnakeWorkers() {
    Snake * snake;
    bool thereAreWinners = false;
    pthread_t  snakesTIds[connections->size];
    Connection * connection;
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
            return true;
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
    return thereAreWinners || (connections->size == 0);
}

void *checkForChangeOfDirections(void * args) {
    Connection * connection;
    int response, direction;
    unsigned char buffer[DELIMITERS_SIZE], directionBuffer[INTEGER_BYTES];

    while (true) {
        for (int i = 0; i < connections->size; i++) {
            // Read if delimiter was passed.
            connection = ((Connection *) connections->data[i]);
            bzero(buffer, DELIMITERS_SIZE);

            // Set to non blocking so that others can also change
            setSocketBlockingEnabled(connection->sockFd, false);
            response = (int) read(connection->sockFd, buffer, DELIMITERS_SIZE);
            setSocketBlockingEnabled(connection->sockFd, true);

            if (response < 0) {
                continue;
            }
            if (strncmp((const char *) buffer, CHANGE_DIRECTION_DELIMITER, DELIMITERS_SIZE) != 0) {
                continue;
            }
            // Read the actual change of direction.

            response = (int) read(connection->sockFd, directionBuffer, INTEGER_BYTES);

            if (response < 0) {
                continue;
            }
            pthread_mutex_lock(&lock);
            direction = (int) connection->clientInfo->snake->direction;
            deserializeInt(directionBuffer, &direction);
            connection->clientInfo->snake->direction = (Direction) direction;
            pthread_mutex_unlock(&lock);
        }
    }
}

void sendEndGameToClients(int sockFd, SnakeStatus status) {
    int response;
    unsigned char buffer[DELIMITERS_SIZE];
    bzero(buffer, DELIMITERS_SIZE);

    if (status == WINNER) {
        serializeCharArray(buffer, WINNER_DELIMITER, DELIMITERS_SIZE);
    } else {
        serializeCharArray(buffer, LOSE_DELIMITER, DELIMITERS_SIZE);
    }
    response = (int) write(sockFd, buffer, DELIMITERS_SIZE);

    if (response < 0) {
        perror("Error writing to socket");
        close(sockFd);
    }
}