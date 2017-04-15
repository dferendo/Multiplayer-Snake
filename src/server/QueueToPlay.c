//
// Created by dylan on 15/04/2017.
//
#include "QueueToPlay.h"
#include "../template/WindowProperties.h"
#include "Server.h"
#include <unistd.h>
#include <pthread.h>
#include "../utility/General.h"

Vector * connections;
pthread_mutex_t lock;

void * addConnectionHost(void *arg) {
    int sockFd = (int)(uintptr_t) arg, requestResponseTemp;
    WINDOW * newClientWindow;
    // Allocate memory to client information
    ClientInfo * clientInfo = (ClientInfo *) malloc(sizeof(ClientInfo));
    // Failed to allocate memory, ignore client
    if (clientInfo == NULL) {
        perror("Failed to allocate memory to client");
        // Close socket.
        close(sockFd);
        pthread_exit(NULL);
    }
    clientInfo->isHost = true;
    // Read user name.
    requestResponseTemp = (int) read(sockFd, clientInfo->name, MAXIMUM_INPUT_STRING);

    // Failed to read, ignore client
    if (requestResponseTemp == -1) {
        perror("Failed to read from socket");
        close(sockFd);
        pthread_exit(NULL);
    }
    // Allocate memory to Connection info
    Connection * connection = (Connection *) malloc(sizeof(Connection));
    if (connection == NULL) {
        perror("Failed to allocate memory to connection");
        // Close socket.
        close(sockFd);
        pthread_exit(NULL);
    }

    connection->sockFd = sockFd;
    connection->clientInfo = clientInfo;

    // Lock connections to add new connection
    pthread_mutex_lock(&lock);
    addItemToVector(connections, connection);
    // Generate window to be sent to the client of all the users.
    newClientWindow = generateWindow(connections);
    // Unlock since connections no longer used.
    pthread_mutex_unlock(&lock);

    requestResponseTemp = (int) write(sockFd, newClientWindow, sizeof(newClientWindow));

    if (requestResponseTemp == -1) {
        perror("Failed to write to the socket");
        close(sockFd);
        pthread_exit(NULL);
    }
    // Host can start game.
}

void * addConnectionNotHost(void *arg) {
    int sockFd = (int)(uintptr_t) arg, requestResponseTemp;
    WINDOW * newClientWindow;
    // Allocate memory to client information
    ClientInfo * clientInfo = (ClientInfo *) malloc(sizeof(ClientInfo));
    // Failed to allocate memory, ignore client
    if (clientInfo == NULL) {
        perror("Failed to allocate memory to client");
        // Close socket.
        close(sockFd);
        pthread_exit(NULL);
    }
    clientInfo->isHost = false;
    // Read user name.
    requestResponseTemp = (int) read(sockFd, clientInfo->name, MAXIMUM_INPUT_STRING);

    // Failed to read, ignore client
    if (requestResponseTemp == -1) {
        perror("Failed to read from socket");
        close(sockFd);
        pthread_exit(NULL);
    }
    // Allocate memory to Connection info
    Connection * connection = (Connection *) malloc(sizeof(Connection));
    if (connection == NULL) {
        perror("Failed to allocate memory to connection");
        // Close socket.
        close(sockFd);
        pthread_exit(NULL);
    }

    connection->sockFd = sockFd;
    connection->clientInfo = clientInfo;

    // Lock connections to add new connection
    pthread_mutex_lock(&lock);
    addItemToVector(connections, connection);
    // Generate window to be sent to the client of all the users.
    newClientWindow = generateWindow(connections);
    // Unlock since connections no longer used.
    pthread_mutex_unlock(&lock);

    requestResponseTemp = (int) write(sockFd, newClientWindow, sizeof(newClientWindow));

    if (requestResponseTemp == -1) {
        perror("Failed to write to the socket");
        close(sockFd);
        pthread_exit(NULL);
    }
    // Only host can start the game.
    pthread_exit(NULL);
}

WINDOW *generateWindow(Vector * connections) {
    // Around 10 characters are needed for the number with the name and if the host
    WINDOW * clientViewWindow = createWindowAtTheCenterOfTheScreen();
    mvwprintw(clientViewWindow, 0, 0, "Players waiting: ");

    for (int i = 0; i < connections->size; i++) {
        char str[MAXIMUM_INPUT_STRING + 10];
        Connection * nextConnection = (Connection *) connections->data[i];
        if (nextConnection->clientInfo->isHost) {
            sprintf(str, "%d) %s (H)", i + 1, nextConnection->clientInfo->name);
            mvwprintw(clientViewWindow, i + 1, 0, str);
        } else {
            sprintf(str, "%d) %s", i + 1, nextConnection->clientInfo->name);
            mvwprintw(clientViewWindow, i + 1, 0, str);
        }
    }
    return clientViewWindow;
}
