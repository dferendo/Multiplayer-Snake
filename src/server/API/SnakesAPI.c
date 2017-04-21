//
// Created by dylan on 21/04/2017.
//

#include <strings.h>
#include "../../utility/Vector.h"
#include "../../utility/Serialize.h"
#include "../../utility/General.h"
#include "../ServerHandle.h"
#include "../Game.h"
#include "../../settings/GameSettings.h"
#include <unistd.h>
#include <pthread.h>
#include <memory.h>

bool sendSnakeDataToClients(Vector * connections) {
    int response;
    Connection * connection;
    // For every snake send the direction, its size as Int and every position for every snake.
    // Also send the amount of snakes.
    size_t size = DELIMITERS_SIZE + INTEGER_BYTES +
            (connections->size * (INTEGER_BYTES * 2));

    // Calculate the size of positions of the snake.
    for (int i = 0; i < connections->size; i++) {
        size += (((Connection *) connections->data[i])->snake->size) * POSITION_BYTES;
    }

    unsigned char buffer[size];
    bzero(buffer, size);

    serializedSnakesFromConnections(buffer, connections);

    // Write to all the clients about the information.
    for (int i = 0; i < connections->size; i++) {
        connection = (Connection *) connections->data[i];

        response = (int) write(connection->sockFd, buffer, size);

        if (response == -1) {
            perror("Failed to write to the socket");
            // Error, close socket, delete connection and indicate the server to
            // resend the data.
            freeConnection(connection);
            deleteItemFromVector(connections, connection);
            return false;
        }
    }
    return true;
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

void * checkForChangeOfDirections(void * args) {
    Vector * connections = ((ChangeDirectionParams *) args)->connections;
    pthread_mutex_t lock = ((ChangeDirectionParams *) args)->lock;
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
            direction = (int) connection->snake->direction;
            deserializeInt(directionBuffer, &direction);
            connection->snake->direction = (Direction) direction;
            pthread_mutex_unlock(&lock);
        }
        sleep(CHARACTER_CHANGE_DIRECTION_TIME_US);
    }
    pthread_exit(NULL);
}