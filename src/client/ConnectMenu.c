//
// Created by dylan on 14/04/2017.
//
#include "ConnectMenu.h"
#include "template/ClientLayout.h"
#include "SnakesGame.h"
#include <strings.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

int sockFd;

void serverManager(int portNumber, char *hostName) {
    int nextAction;

    if (!connectToServer(&sockFd, portNumber, hostName)) {
        return;
    }
    // For CTRL + C,
    // The signals SIGKILL and SIGSTOP cannot be caught, blocked, or ignored - man 7 signal
    // Thus kill command is ignored.
    signal(SIGINT, terminateClient);

    while (true) {
        nextAction = gameManager(sockFd);
        if (nextAction < 0) {
            break;
        }
    }
}

bool connectToServer(int * sockFd, int portNumber, char * hostName) {
    struct sockaddr_in serverAddress;
    struct hostent * server;

    // Connect to server
    *sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (*sockFd == -1) {
        perror("Error encountered when opening the socket");
        exit(1);
    }
    // Get the server name
    server = gethostbyname(hostName);
    if (server == NULL) {
        printError(ERROR_NO_HOST);
        return false;
    }

    // Populate serverAddress structure
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy(server->h_addr,
          (char *) &serverAddress.sin_addr.s_addr,
          (size_t) server->h_length);
    serverAddress.sin_port = htons((uint16_t) portNumber);

    // Connect to server
    if (connect(*sockFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        printError(ERROR_CONNECTION_FAILED);
        return false;
    }
    return true;
}

void terminateClient(int sig) {
    // Close socket
    close(sockFd);
    // End ncurses
    endwin();
    printf("Client disconnected.\n");
    exit(0);
}
