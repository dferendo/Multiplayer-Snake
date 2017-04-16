//
// Created by dylan on 14/04/2017.
//
#include "StartGame.h"
#include "../template/WindowProperties.h"
#include "../template/ClientLayout.h"
#include "../utility/General.h"
#include "../utility/Serialize.h"
#include <unistd.h>
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
    Vector * connections = initVector();

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
            if (printErrorAndOfferRetry(ERROR_NO_HOST)) {
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
            if (printErrorAndOfferRetry(ERROR_CONNECTION_FAILED)) {
                continue;
            } else {
                break;
            }
        } else {
            // Connection successful
            // Send Name To server
            writeNameToSocket(sockFd, playerName);
            // Get connections the server has
            readConnectionsFromSocket(sockFd, connections);
            // Display connections
            WINDOW * queue = displayConnections(connections);
            wrefresh(queue);
            getch();
            break;
        }
    }
}

void getInput(char *name, char *serverName, char *port) {
    bzero(name, MAXIMUM_INPUT_STRING);
    bzero(serverName, MAXIMUM_INPUT_STRING);
    bzero(port, MAXIMUM_INPUT_STRING);
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
    name[MAXIMUM_INPUT_STRING - 1] = '\0';
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

bool printErrorAndOfferRetry(char *errorMessage) {
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

void writeNameToSocket(int socketFileDescriptor, char * name) {
    int response;
    unsigned char buffer[MAXIMUM_INPUT_STRING];
    bzero(buffer, MAXIMUM_INPUT_STRING);

    serializeCharArray(buffer, name, MAXIMUM_INPUT_STRING);
    response = (int) write(socketFileDescriptor, buffer, MAXIMUM_INPUT_STRING);

    if (response == -1) {
        perror("Error reading from socket");
        close(socketFileDescriptor);
        exit(1);
    }
}

void readConnectionsFromSocket(int socketFileDescriptor, Vector * connections) {
    int response;
    int amountOfConnections;
    size_t connectionsSize;
    unsigned char bufferInteger[INTEGER_BYTES];
    bzero(bufferInteger, INTEGER_BYTES);

    response = (int) read(socketFileDescriptor, bufferInteger, INTEGER_BYTES);

    if (response == -1) {
        perror("Error when reading from socket");
        close(socketFileDescriptor);
        exit(1);
    }

    deserializeInt(bufferInteger, &amountOfConnections);
    connectionsSize = (size_t) (amountOfConnections * CONNECTION_BYTES);

    unsigned char buffer[connectionsSize];
    bzero(buffer, connectionsSize);

    response = (int) read(socketFileDescriptor, buffer, connectionsSize);

    if (response == -1) {
        perror("Error when reading from socket");
        close(socketFileDescriptor);
        exit(1);
    }
    // De-serialize the connections and put them in a vector
    deserializeVectorOfConnections(buffer, connections, amountOfConnections);
}

WINDOW *displayConnections(Vector * connections) {
    WINDOW * window = createWindowAtTheCenterOfTheScreen();
    char lineWidth[MAXIMUM_INPUT_STRING + PLAY_GAME_MENU_LENGTH];

    for (int i = 0; i < connections->size; i++) {
        struct Connection * connection = connections->data[i];

        if (connection->clientInfo->isHost) {
            sprintf(lineWidth, "%d) %s (H)", i + 1, connection->clientInfo->name);
        } else {
            sprintf(lineWidth, "%d) %s", i + 1, connection->clientInfo->name);
        }
        mvwprintw(window, i, 2, lineWidth);
    }
    return window;
}
