//
// Created by dylan on 18/04/2017.
//
#include <pthread.h>
#include <strings.h>
#include "Server.h"
#include "Game.h"
#include "Food.h"
#include "../utility/Serialize.h"
#include <unistd.h>

Vector * connections;
Vector * foods;
pthread_mutex_t lock;

void gameInitialize() {
    pthread_t foodThread;
    // Where all the food will be placed.
    foods = initVector();
    // Send initial snake information
    sendInitialSnakeInformationToClients();
    // Create thread for food.
    if (pthread_create(&foodThread, NULL, generateFood, NULL) != 0) {
        perror("Could not create a food thread.");
        return;
    }

    while (true) {

    }
}

void sendInitialSnakeInformationToClients() {
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
