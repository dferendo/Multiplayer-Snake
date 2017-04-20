//
// Created by dylan on 20/04/2017.
//

#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include "../../settings/GameSettings.h"
#include "../../utility/Serialize.h"
#include "../../utility/General.h"

bool readNameFromSocket(int socketFileDescriptor, char * name) {
    unsigned char buffer[MAXIMUM_INPUT_STRING];
    int requestResponseTemp;
    bzero(buffer, MAXIMUM_INPUT_STRING);

    requestResponseTemp = (int) read(socketFileDescriptor, buffer, MAXIMUM_INPUT_STRING);

    if (requestResponseTemp == -1) {
        perror("Failed to read to the socket");
        close(socketFileDescriptor);
        return false;
    }
    deserializeCharArray(buffer, name, MAXIMUM_INPUT_STRING);
    return true;
}

bool writeConnectionsToSockets(Vector *connections) {
    int requestResponseTemp;
    // 2 Integer bytes are passed before tell the amount of connections and the
    // size of the initial snake.
    size_t size = (CONNECTION_BYTES_NO_SNAKE * connections->size) + INTEGER_BYTES + DELIMITERS_SIZE;
    // Create the number of bytes needed.
    unsigned char buffer[size];
    bzero(buffer, size);

    // Add delimiter so that clients know that there is data to be read.
    serializedVectorOfConnectionsDelimiter(buffer, connections);

    // Write to all the other clients that someone joined.
    for (int i = 0; i < connections->size; i++) {
        struct Connection * connection = (Connection *) connections->data[i];

        requestResponseTemp = (int) write(connection->sockFd, buffer, size);

        if (requestResponseTemp == -1) {
            perror("Failed to write to the socket");
            // Terminate connection, remove connection from connections.
            close(connection->sockFd);
            freeConnection(connection);
            deleteItemFromVector(connections, connection);
            return false;
        }
    }
    return true;
}

void readStartGameFromHost(int socketFileDescriptor) {
    int response;
    unsigned char buffer[DELIMITERS_SIZE];

    while (true) {
        bzero(buffer, DELIMITERS_SIZE);

        response = (int) read(socketFileDescriptor, buffer, DELIMITERS_SIZE);

        if (response == -1) {
            perror("Error when reading from socket");
            close(socketFileDescriptor);
            exit(1);
        }
        // Host wants to start Game.
        if (strncmp((const char *) buffer, HOST_STARTS_GAME_DELIMITER, DELIMITERS_SIZE) == 0) {
            break;
        }
        usleep(HOST_START_GAME_DELAY);
    }
}

void writeStartingGameToConnectionsExceptHost(Vector *connections) {
    int response;
    unsigned char buffer[DELIMITERS_SIZE];
    bzero(buffer, DELIMITERS_SIZE);

    serializeCharArray(buffer, HOST_STARTS_GAME_DELIMITER, DELIMITERS_SIZE);

    for (int i = 0; i < connections->size; i++) {
        Connection * temp = (Connection *) connections->data[i];
        // Skip host since he made the call he already knows.
        if (!temp->clientInfo->isHost) {
            response = (int) write(temp->sockFd, buffer, DELIMITERS_SIZE);

            if (response == -1) {
                perror("Error reading from socket");
                close(temp->sockFd);
            }
        }
    }
}
