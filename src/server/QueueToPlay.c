//
// Created by dylan on 15/04/2017.
//
#include "QueueToPlay.h"
#include "../template/WindowProperties.h"
#include <unistd.h>
#include <pthread.h>
#include <strings.h>
#include <memory.h>
#include "../utility/Serialize.h"

Vector * connections;
pthread_mutex_t lock;

void * initNewConnection(void *arg) {
    int socketFileDescriptor = (int)(uintptr_t) ((CreateConnectThreadArguments *) arg)->sockFd;
    short isHost = ((CreateConnectThreadArguments *) arg)->isHost;
    char buffer[MAXIMUM_INPUT_STRING];
    Connection * connection;

    // Read name
    readNameFromSocket(socketFileDescriptor, buffer);
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

void readNameFromSocket(int socketFileDescriptor, char * name) {
    unsigned char buffer[MAXIMUM_INPUT_STRING];
    int requestResponseTemp;
    bzero(buffer, MAXIMUM_INPUT_STRING);

    requestResponseTemp = (int) read(socketFileDescriptor, buffer, MAXIMUM_INPUT_STRING);

    if (requestResponseTemp == -1) {
        perror("Failed to read to the socket");
        close(socketFileDescriptor);
        pthread_exit(NULL);
    }
    deserializeCharArray(buffer, name, MAXIMUM_INPUT_STRING);
}

void writeConnectionsToSockets(Vector *connections) {
    int requestResponseTemp;
    // 4 is the integer to tell the amount of connections
    size_t size = (CONNECTION_BYTES * connections->size) + 4;
    // Create the number of bytes needed.
    unsigned char buffer[size];
    bzero(buffer, size);

    serializeVectorOfConnections(buffer, connections);

    // Write to all the other clients that someone joined.
    for (int i = 0; i < connections->size; i++) {
        struct Connection * connection = (Connection *) connections->data[i];

        requestResponseTemp = (int) write(connection->sockFd, buffer, size);

        if (requestResponseTemp == -1) {
            perror("Failed to write to the socket");
            close(connection->sockFd);
        }
    }
}
