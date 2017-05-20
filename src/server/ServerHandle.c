//
// Created by dylan on 21/04/2017.
//
#include <pthread.h>
#include <strings.h>
#include "ServerHandle.h"
#include "../utility/General.h"
#include "API/SnakesAPI.h"
#include <unistd.h>
#include <netinet/in.h>

pthread_mutex_t lock;
Vector * foods;
Vector * connections;
int sockFd;

void * serverInit(void * args) {
    uint16_t portNumber = ((ServerParams *) args)->portNumber;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientSize;

    // Create socket
    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd == -1) {
        perror("Error opening socket.");
        exit(1);
    }

    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    // Accepts any addresses
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNumber);

    // Binding
    if (bind(sockFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error on binding");
        exit(1);
    }

    listen(sockFd, 0);
    clientSize = sizeof(clientAddress);
    acceptClients(sockFd, (struct sockaddr *) &clientAddress, &clientSize);

    pthread_exit(NULL);
}

void acceptClients(int sockFd, struct sockaddr * clientAddress, socklen_t * clientSize) {
    int newSockFd;
    pthread_t clientThread;

    // Wait for connections
    while (true) {
        // Accept connection
        newSockFd = accept(sockFd, clientAddress, clientSize);

        // Failed connection, ignore client
        if (newSockFd == -1) {
            perror("Error on client accept");
            continue;
        }
        CreateConnectThreadArguments * args = (CreateConnectThreadArguments *)
                malloc(sizeof(CreateConnectThreadArguments));

        // Failed malloc, ignore client
        if (args == NULL) {
            perror("Error on malloc arguments");
            close(newSockFd);
            exit(1);
        }

        args->sockFd = newSockFd;

        if (pthread_create(&clientThread, NULL, initNewConnection, args) != 0) {
            perror("Could not create a worker thread.");
            free(args);
            close(newSockFd);
            exit(1);
        }
    }
}

void * initNewConnection(void *arg) {
    int socketFileDescriptor = ((CreateConnectThreadArguments *) arg)->sockFd;

    Connection * connection;
    bool error;

    // Lock required to add on the connections vector
    pthread_mutex_lock(&lock);
    // Create Connection
    connection = createConnection(socketFileDescriptor, connections, foods);

    // Failed to create connection, stop the client and stop the thread.
    if (connection == NULL) {
        pthread_mutex_unlock(&lock);
        close(socketFileDescriptor);
        pthread_exit(NULL);
    }

    // Failed to re-alloc
    if (addItemToVector(connections, connection) < 0) {
        pthread_mutex_unlock(&lock);
        // Error
        freeDataOfConnection(connection);
        pthread_exit(NULL);
    }

    // Send unique ID
    if (!writeUserUniqueID(socketFileDescriptor, connection->uniqueID)) {
        // Failed to send, ignore connection
        freeDataOfConnection(connection);
        deleteItemFromVector(connections, connection);
        pthread_exit(NULL);
    }

    do {
        // Send data again, if a connection is lost re-send the data.
        error = sendSnakeDataToClients(connections);
        // Send Food data as well
        error |= writeFoodDataToClients(connections, foods);;
    } while (!error);

    // Unlock since connections no longer used.
    pthread_mutex_unlock(&lock);

    free(arg);
    pthread_exit(NULL);
}

Connection * createConnection(int socketFileDescriptor, Vector * connections, Vector * foods) {
    static unsigned int uniqueID = 0;
    Snake * snake;
    // Allocate memory to Connection info
    Connection * connection = (Connection *) malloc(sizeof(Connection));
    if (connection == NULL) {
        perror("Failed to allocate memory to connection");
        return NULL;
    }
    snake = createSnake(connections, foods, false, 0);

    if (snake == NULL) {
        return NULL;
    }

    connection->sockFd = socketFileDescriptor;
    connection->snake = snake;
    connection->uniqueID = uniqueID++;
    return connection;
}