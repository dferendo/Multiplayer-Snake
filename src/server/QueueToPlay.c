//
// Created by dylan on 15/04/2017.
//
#include "QueueToPlay.h"
#include <unistd.h>
#include <pthread.h>
#include <strings.h>
#include <memory.h>
#include "../utility/Serialize.h"
#include "../template/GameSettings.h"
#include "Game.h"

Vector * connections;
Vector * initialPositions;
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
            close(connection->sockFd);
        }
    }
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
