//
// Created by dylan on 21/04/2017.
//
#include <pthread.h>
#include <strings.h>
#include "ServerHandle.h"
#include "../utility/General.h"
#include "API/SnakesAPI.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void * serverInit(void * args) {
    ServerParams * serverParams = (ServerParams *) args;
    uint16_t portNumber = serverParams->portNumber;
    int sockFd;
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
    acceptClients(sockFd, (struct sockaddr *) &clientAddress, &clientSize, serverParams);

    pthread_exit(NULL);
}

void acceptClients(int sockFd, struct sockaddr * clientAddress, socklen_t * clientSize,
                   ServerParams * serverParams) {
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
        args->lock = serverParams->lock;
        args->connections = serverParams->connections;
        args->foods = serverParams->foods;

        // Main param is no longer needed
        free(serverParams);

        if (pthread_create(&clientThread, NULL, initNewConnection, args) != 0) {
            perror("Could not create a worker thread.");
            free(args);
            close(newSockFd);
            exit(1);
        }
    }
}

void * initNewConnection(void *arg) {
    Vector * connections = ((CreateConnectThreadArguments *) arg)->connections;
    Vector * foods = ((CreateConnectThreadArguments *) arg)->foods;
    int socketFileDescriptor = ((CreateConnectThreadArguments *) arg)->sockFd;
    pthread_mutex_t lock = ((CreateConnectThreadArguments *) arg)->lock;

    Connection * connection;
    bool error;

    // Lock required to add on the connections vector
    pthread_mutex_lock(&lock);
    // Create Connection
    connection = createConnection(socketFileDescriptor, connections, foods);

    // Failed to create connection, stop the client and stop the thread.
    if (connection == NULL) {
        close(socketFileDescriptor);
        pthread_exit(NULL);
    }

    // Lock connections to add new connection
    if (addItemToVector(connections, connection) < 0) {
        // Error
        freeConnection(connection);
        pthread_exit(NULL);
    }

    do {
        // Send data again, if a connection is lost re-send the data.
        error = sendSnakeDataToClients(connections);

        if (connections->size == 0) {
            pthread_exit(NULL);
        }
    } while (!error);

    // Unlock since connections no longer used.
    pthread_mutex_unlock(&lock);

    free(arg);
    pthread_exit(NULL);
}

Connection * createConnection(int socketFileDescriptor, Vector * connections, Vector * foods) {
    Snake * snake;
    // Allocate memory to Connection info
    Connection * connection = (Connection *) malloc(sizeof(Connection));
    if (connection == NULL) {
        perror("Failed to allocate memory to connection");
        return NULL;
    }
    snake = createSnake(connections, foods);

    if (snake == NULL) {
        return NULL;
    }

    connection->sockFd = socketFileDescriptor;
    connection->snake = snake;
    return connection;
}