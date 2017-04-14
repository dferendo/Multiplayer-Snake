//
// Created by dylan on 14/04/2017.
//
#include "StartGame.h"
#include "../template/WindowProperties.h"
#include "../template/ClientLayout.h"
#include <stdlib.h>
#include <strings.h>

const char * const SERVER_REQUIRED[PLAY_GAME_MENU_ITEMS] = {
        "Player Name: ",
        "Server Name: ",
        "Port Number: "
};

void startGameInit() {
    // User Inputs
    char playerName[MAXIMUM_INPUT_STRING], serverName[MAXIMUM_INPUT_STRING],
            port[MAXIMUM_INPUT_STRING];
    // Variables needed to connect to server
    int sockFd, portNumber;
    struct sockaddr_in serverAddress;
    struct hostent * server;

    while (true) {
        getInput(playerName, serverName, port);
        // Connect to server
        portNumber = atoi(port);
        // AF_INET address family that is used to designate the type
        // of addresses. (IP v4 addresses)
        // SOCK_STREAM Provides sequenced, reliable, bidirectional, connection-mode byte streams, and may provide a
        // transmission mechanism for out-of-band data.
        // Default protocol.
        sockFd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockFd == -1) {
            perror("Error encountered when opening the socket.");
            exit(1);
        }
        // Get the server name
        server = gethostbyname(serverName);
        if (!server) {
            // Check if user wants to re-try.
            if (printError(ERROR_NO_HOST)) {
                continue;
            } else {
                break;
            }
        }

        // Populate serverAddress structure
        bzero((char *) &serverAddress, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        bcopy(server -> h_addr,
              (char *) &serverAddress.sin_addr.s_addr,
              (size_t) server -> h_length);
        serverAddress.sin_port = htons((uint16_t) portNumber);

        // Connect to server
        if (connect(sockFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
            // Check if user wants to re-try.
            if (printError(ERROR_CONNECTION_FAILED)) {
                continue;
            } else {
                break;
            }
        }
    }
}

void getInput(char *name, char *serverName, char *port) {
    // Display Input name Window
    WINDOW * detailsInput = createWindowAtTheCenterOfTheScreen();

    for (int i = 0; i < PLAY_GAME_MENU_ITEMS; i++) {
        mvwprintw(detailsInput, i, 0, SERVER_REQUIRED[i]);
    }

    wrefresh(detailsInput);
    // Enable user to write on screen
    echo();
    // Get name
    wmove(detailsInput, 0, PLAY_GAME_MENU_LENGTH);
    wgetstr(detailsInput, name);
    // Get Server name
    wmove(detailsInput, 1, PLAY_GAME_MENU_LENGTH);
    wgetstr(detailsInput, serverName);
    // Get port number
    wmove(detailsInput, 2, PLAY_GAME_MENU_LENGTH);
    wgetstr(detailsInput, port);
    // Clear window
    wclear(detailsInput);
    wrefresh(detailsInput);
    delwin(detailsInput);
}

bool printError(char *errorMessage) {
    WINDOW * window = createWindowAtTheCenterOfTheScreen();

    mvwprintw(window, 0, 0, errorMessage);
    mvwprintw(window, 1, 0, "Retry? (Y/n)");
    wrefresh(window);
    int retry = getch();
    if (retry == 'Y' || retry == 'y') {
        return true;
    }
    return false;
}

WINDOW * createWindowAtTheCenterOfTheScreen() {
    WINDOW * tempWindow;
    // StartingX and Y put in centre (Approx)
    int height = 4, width = MAXIMUM_INPUT_STRING + PLAY_GAME_MENU_LENGTH,
            startingX = (COLUMN / 2) - (width / 2), startingY = (ROW / 2) - height;
    // Create new window where main menu will be placed.
    tempWindow = newwin(height, width, startingY, startingX);
    return tempWindow;
}
