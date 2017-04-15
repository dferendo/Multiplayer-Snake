//
// Created by dylan on 14/04/2017.
//
#include "Server.h"
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

Vector * connections;

int main(int argc, char *argv[]) {
    // Variables needed
    int sockFd, portNumber, clientSize, newSockFd;
    struct sockaddr_in serverAddress, clientAddress;
    connections = initVector();
    // Create socket
    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd == -1) {
        perror("Error opening socket.");
        exit(1);
    }

    bzero((char *) &serverAddress, sizeof(serverAddress));
    // TODO check;
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

    while (true) {
        // Accept connection
        newSockFd = accept(sockFd, (struct sockaddr *) &clientAddress, &clientSize);

        if (newSockFd == -1) {
            perror("Error on client accept.");
        }

    }

    close(sockFd);
    return 0;
}