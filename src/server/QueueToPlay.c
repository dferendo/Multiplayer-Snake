//
// Created by dylan on 15/04/2017.
//
#include "QueueToPlay.h"
#include "Server.h"
#include "../template/WindowProperties.h"
#include <unistd.h>
#include <pthread.h>
#include "../utility/General.h"

Vector * connections;
pthread_mutex_t lock;

void * initNewConnection(void *arg) {
    int socketFileDescriptor = (int)(uintptr_t) ((CreateConnectThreadArguments *) arg)->sockFd;
    bool isHost = (bool) ((CreateConnectThreadArguments *) arg)->isHost;
    char * buffer;
    Connection * connection;

    // Read name
    buffer = readNameFromSocket(socketFileDescriptor);
    // Create Connection
    connection = createConnection(isHost, buffer, socketFileDescriptor);

    // Failed to create connection, stop the client.
    if (connection == NULL) {
        close(socketFileDescriptor);
        pthread_exit(NULL);
    }

    // Lock connections to add new connection
    pthread_mutex_lock(&lock);
    addItemToVector(connections, connection);
    writeConnectionsToSockets(connections);
    // Unlock since connections no longer used.
    pthread_mutex_unlock(&lock);

    if (!isHost) {
        pthread_exit(NULL);
    }
    // Only host can start the game.
    pthread_exit(NULL);
}

Connection * createConnection(bool isHost, char * name, int socketFileDescriptor) {
    ClientInfo * clientInfo = (ClientInfo *) malloc(sizeof(ClientInfo));
    // Failed to allocate memory, ignore client
    if (clientInfo == NULL) {
        perror("Failed to allocate memory to client");
        // Close socket.
        return NULL;
    }
    clientInfo->isHost = isHost;
    clientInfo->name = name;
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

char * readNameFromSocket(int socketFileDescriptor) {
    char * buffer = NULL;
    int requestResponseTemp;
    size_t size = sizeof(char) * MAXIMUM_INPUT_STRING;

    requestResponseTemp = (int) read(socketFileDescriptor, buffer, size);

    if (requestResponseTemp == -1) {
        perror("Failed to read to the socket");
        close(socketFileDescriptor);
        pthread_exit(NULL);
    }
    return buffer;
}

void writeConnectionsToSockets(Vector *connections) {
    int requestResponseTemp;
    // Calculate the size of the vector
    size_t size = sizeof(Vector) + (sizeof(*connections->data) * connections->size) +
            (sizeof(connections->size) * sizeof(ClientInfo));

    // Write to all the other clients that someone joined.
    for (int i = 0; i < connections->size; i++) {
        struct Connection * connection = (Connection *) connections->data[i];

        requestResponseTemp = (int) write(connection->sockFd, connections, size);

        if (requestResponseTemp == -1) {
            perror("Failed to write to the socket");
            close(connection->sockFd);
        }
    }
}
