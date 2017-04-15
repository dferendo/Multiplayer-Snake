//
// Created by dylan on 14/04/2017.
//
#include "Server.h"
#include <pthread.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include "QueueToPlay.h"

Vector * connections;

int main(int argc, char *argv[]) {
    // Variables needed
    int sockFd, portNumber, clientSize, newSockFd;
    struct sockaddr_in serverAddress, clientAddress;
    pthread_t clientThread;
    connections = initVector();
    bool hostEstablish = false;
    // Create socket
    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd == -1) {
        perror("Error opening socket.");
        exit(1);
    }

    bzero((char *) &serverAddress, sizeof(serverAddress));
    if (argc < 2) {
        perror("Port number was not passed.");
        exit(1);
    }
    portNumber = atoi(argv[1]);
    serverAddress.sin_family = AF_INET;
    // Accepts any addresses
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons((uint16_t) portNumber);

    // Binding
    if (bind(sockFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error on binding");
    }

    listen(sockFd, 0);
    clientSize = sizeof(clientAddress);

    // Wait for connections
    while (true) {
        // Accept connection
        newSockFd = accept(sockFd, (struct sockaddr *) &clientAddress, &clientSize);

        // Failed connection, ignore client
        if (newSockFd == -1) {
            perror("Error on client accept.");
            continue;
        }
        CreateConnectThreadArguments args = (CreateConnectThreadArguments *)
                malloc(sizeof(CreateConnectThreadArguments));

        // Failed connection, ignore client
        if (args == NULL) {
            perror("Error on malloc arguments.");
            close(newSockFd);
            continue;
        }
        args.sockFd = newSockFd;

        if (!hostEstablish) {
            hostEstablish = true;
            args.isHost = true;
        } else {
            args.isHost = false;
        }

        if (pthread_create(&clientThread, NULL, initNewConnection, args) != 0) {
            perror("Could not create a worker thread.");
            free(args);
            close(newSockFd);
        }
    }

}