//
// Created by dylan on 14/04/2017.
//
#include "QueueMenu.h"
#include "../template/ClientLayout.h"
#include "../utility/General.h"
#include "SnakesGame.h"
#include "QueueAPI.h"
#include <unistd.h>
#include <strings.h>

void serverConnection() {
    int sockFd;
    char playerName[MAXIMUM_INPUT_STRING];

    // Connection failed.
    if (!connectToServer(&sockFd, playerName)) {
        return;
    }
    // TODO: check if name is unique.
    // Send Name To server
    writeNameToSocket(sockFd, playerName);
    // Continue updating the queue players until host starts game.
    waitUntilHostStartsGame(&sockFd, playerName);
}

bool connectToServer(int * sockFd, char * playerName) {
    // User Inputs
    char serverName[MAXIMUM_INPUT_STRING],
            port[MAXIMUM_INPUT_STRING];
    // Variables needed to connect to server
    int portNumber;
    struct sockaddr_in serverAddress;
    struct hostent * server;

    while (true) {
        // Asks for input and try to connect to the server.
        getInput(playerName, serverName, port);
        // Connect to server
        portNumber = atoi(port);
        *sockFd = socket(AF_INET, SOCK_STREAM, 0);

        if (*sockFd == -1) {
            perror("Error encountered when opening the socket.");
            exit(1);
        }
        // Get the server name
        server = gethostbyname(serverName);
        if (!server) {
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
    // If while break, that means the user does not want to re-try thus go back to main screen.
    return false;
}

void waitUntilHostStartsGame(int *sockFd, char * playerId) {
    Vector * connections = NULL;
    bool isHost = false;
    // Set getCh delay.
    halfdelay(WAIT_INPUT_TIME_FOR_HOST_TO_START_GAME);

    while (true) {

        int nextAction = readDelimiterQueue(sockFd);

        if (nextAction == 1) {
//            deleteWindow(window);
//            delwin(window);
            gameInit(connections, *sockFd);
            break;
        } else if (nextAction == 2) {
            if (connections != NULL) {
                clearConnectionVector(connections);
            }
            // Get connections the server has
            connections = readConnectionsFromSocket(*sockFd);
            isHost = checkIfHost(connections, playerId);
//            deleteWindow(window);
//            generateWindowForWaitingInQueue(connections, window, isHost);
        } else if (isHost) {
            noecho();
            int input = getch();
            // Start Game
            if (input == 'S') {
                writeStartGameToSocket(sockFd);
                // Remove delay from char.
                cbreak();
//                deleteWindow(window);
//                delwin(window);
                gameInit(connections, *sockFd);
                break;
            }
        } else {
            sleep(SLEEP_WHEN_NO_HOST_QUEUE_SEC);
        }
    }
}

void clearConnectionVector(Vector * oldVector) {

    // ClientInfo will not be de-allocated by deleteVector().
    for (int i = 0; i < oldVector->size; i++) {
        Connection * temp = (Connection *) oldVector->data[i];
        // Clear the Client.
        free(temp->clientInfo);
    }
    deleteVector(oldVector);
}
