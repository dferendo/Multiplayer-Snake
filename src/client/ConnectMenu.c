//
// Created by dylan on 14/04/2017.
//
#include "ConnectMenu.h"
#include "template/ClientLayout.h"
#include "../utility/General.h"
#include "SnakesGame.h"
#include "../settings/GameSettings.h"
#include <unistd.h>
#include <strings.h>

void serverConnection() {
    int sockFd;

    if (!connectToServer(&sockFd)) {
        return;
    }

    gameInit(sockFd);
}

bool connectToServer(int * sockFd) {
    // User Inputs
    char serverName[MAXIMUM_INPUT_STRING],
            port[MAXIMUM_INPUT_STRING];
    // Variables needed to connect to server
    int portNumber;
    struct sockaddr_in serverAddress;
    struct hostent * server;

    while (true) {
        // Asks for input and try to connect to the server.
        getInput(serverName, port);
        // Connect to server
        portNumber = atoi(port);
        *sockFd = socket(AF_INET, SOCK_STREAM, 0);

        if (*sockFd == -1) {
            perror("Error encountered when opening the socket");
            exit(1);
        }
        // Get the server name
        server = gethostbyname(serverName);
        if (server == NULL) {
            // Check if user wants to re-try.
            if (printErrorAndOfferRetry(ERROR_NO_HOST)) {
                continue;
            } else {
                break;
            }
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
            // Check if user wants to re-try.
            if (printErrorAndOfferRetry(ERROR_CONNECTION_FAILED)) {
                continue;
            } else {
                break;
            }
        } else {
            return true;
        }
    }
    return false;
}