//
// Created by dylan on 15/04/2017.
//
#include "General.h"
#include "LinkedList.h"
#include "../server/ServerHandle.h"
#include <string.h>
#include <unistd.h>

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
    deleteLinkedListPosition(connection->snake->positions);
    // Free Snake
    free(connection->snake);
    // Close socket
    close(connection->sockFd);
}