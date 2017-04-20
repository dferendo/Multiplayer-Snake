//
// Created by dylan on 15/04/2017.
//
#include "General.h"
#include <string.h>
#include <unistd.h>

bool checkIfHost(Vector *connections, char *playerID) {

    for (int i = 0; i < connections->size; i++) {
        Connection * connection = (Connection *) connections->data[i];

        if (strcmp(connection->clientInfo->name, playerID) == 0 &&
            connection->clientInfo->isHost) {
            return true;
        }
    }
    return false;
}

bool setSocketBlockingEnabled(int sockFd, bool blocking) {
    int flags;

    if (sockFd < 0) {
        return false;
    }
    flags = fcntl(sockFd, F_GETFL, 0);
    if (flags < 0) {
        return false;
    }
    flags = blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
    return (fcntl(sockFd, F_SETFL, flags) == 0) ? true : false;
}

void freeConnection(Connection *connection) {
    // Delete linked list with all the positions
    deleteLinkedListPosition(connection->clientInfo->snake->positions);
    // Free Snake
    free(connection->clientInfo->snake);
    // Free Client
    free(connection->clientInfo);
    // Close socket
    close(connection->sockFd);
}

void freeConnectionNoSnake(Connection * connection) {
    // Free Client
    free(connection->clientInfo);
    // Close socket
    close(connection->sockFd);
}