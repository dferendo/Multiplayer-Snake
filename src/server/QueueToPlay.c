//
// Created by dylan on 15/04/2017.
//
#include "QueueToPlay.h"
#include <unistd.h>
#include <pthread.h>
#include <memory.h>
#include "Game.h"
#include "API/ServerQueueAPI.h"
#include "../utility/General.h"

Vector * connections;
Vector * initialPositions;
pthread_mutex_t lock;

void * initNewConnection(void *arg) {
    int socketFileDescriptor = (int)(uintptr_t) ((CreateConnectThreadArguments *) arg)->sockFd;
    short isHost = ((CreateConnectThreadArguments *) arg)->isHost;
    char buffer[MAXIMUM_INPUT_STRING];
    Connection * connection;
    bool errorHandle;

    // Read name
    if (!readNameFromSocket(socketFileDescriptor, buffer)) {
        // Free arguments and close thread.
        free(arg);
        pthread_exit(NULL);
    }
    // Create Connection
    connection = createConnection(isHost, buffer, socketFileDescriptor);

    // Failed to create connection, stop the client.
    if (connection == NULL) {
        close(socketFileDescriptor);
        pthread_exit(NULL);
    }

    // Lock connections to add new connection
    pthread_mutex_lock(&lock);
    if (addItemToVector(connections, connection) < 0) {
        // Error
        close(socketFileDescriptor);
        freeConnection(connection);
        pthread_exit(NULL);
    }

    do {
        // If failed, remove the failed connection and re-send that data
        // unless there are no more connections.
        errorHandle = writeConnectionsToSockets(connections);
    } while (!errorHandle);

    // Unlock since connections no longer used.
    pthread_mutex_unlock(&lock);

    if (isHost) {
        readStartGameFromHost(socketFileDescriptor);
        // Inform others game is starting.
        pthread_mutex_lock(&lock);
        writeStartingGameToConnectionsExceptHost(connections);
        pthread_mutex_unlock(&lock);
        // Start game
        gameInitialize();
    }
    // Only host can start the game.
    free(arg);
    pthread_exit(NULL);
}

Connection * createConnection(short isHost, char * name, int socketFileDescriptor) {
    ClientInfo * clientInfo = (ClientInfo *) malloc(sizeof(ClientInfo));
    // Failed to allocate memory, ignore client
    if (clientInfo == NULL) {
        perror("Failed to allocate memory to client");
        // Close socket.
        return NULL;
    }
    clientInfo->isHost = isHost;
    strcpy(clientInfo->name, name);

    // Lock the initial positions.
    pthread_mutex_lock(&lock);
    clientInfo->snake = createSnake(initialPositions);
    pthread_mutex_unlock(&lock);

    // Allocate memory to Connection info
    Connection * connection = (Connection *) malloc(sizeof(Connection));
    if (connection == NULL) {
        perror("Failed to allocate memory to connection");
        free(clientInfo);
        return NULL;
    }

    connection->sockFd = socketFileDescriptor;
    connection->clientInfo = clientInfo;
    return connection;
}