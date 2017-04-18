//
// Created by dylan on 14/04/2017.
//
#include "Server.h"
#include <pthread.h>
#include <unistd.h>
#include <strings.h>
#include "QueueToPlay.h"

Vector * connections;
Vector * initialPositions;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Port number was not passed");
        exit(1);
    }

    serverInit((uint16_t) atoi(argv[1]));
}

void serverInit(uint16_t portNumber) {
    int sockFd;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientSize;
    // Vectors needed
    connections = initVector();
    initialPositions = initVector();
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
}

void acceptClients(int sockFd, struct sockaddr * clientAddress, socklen_t * clientSize) {
    bool hostEstablish = false;
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

        // Failed connection, ignore client
        if (args == NULL) {
            perror("Error on malloc arguments");
            close(newSockFd);
            continue;
        }
        args->sockFd = newSockFd;

        if (!hostEstablish) {
            hostEstablish = true;
            args->isHost = true;
        } else {
            args->isHost = false;
        }

        if (pthread_create(&clientThread, NULL, initNewConnection, args) != 0) {
            perror("Could not create a worker thread.");
            free(args);
            close(newSockFd);
        }
    }
}
