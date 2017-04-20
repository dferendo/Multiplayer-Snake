//
// Created by dylan on 14/04/2017.
//
#include "QueueMenu.h"
#include "../template/ClientLayout.h"
#include "../utility/General.h"
#include "../utility/Serialize.h"
#include "SnakesGame.h"
#include <unistd.h>
#include <strings.h>
#include <string.h>

const char * const SERVER_REQUIRED[PLAY_GAME_MENU_ITEMS] = {
        "Player Name: ",
        "Server Name: ",
        "Port Number: "
};

void queueConnectionManager(WINDOW *window) {
    // SockFd will be the unique identifier of the users.
    int sockFd;
    char playerName[MAXIMUM_INPUT_STRING];

    // Connection failed.
    if (!connectToServer(window, &sockFd, playerName)) {
        return;
    }

    // Send Name To server
    writeNameToSocket(sockFd, playerName);
    // Continue updating the queue players until host starts game.
    waitUntilHostStartsGame(window, &sockFd, playerName);
}

bool connectToServer(WINDOW *window, int * sockFd, char * playerName) {
    // User Inputs
    char serverName[MAXIMUM_INPUT_STRING],
            port[MAXIMUM_INPUT_STRING];
    // Variables needed to connect to server
    int portNumber;
    struct sockaddr_in serverAddress;
    struct hostent * server;

    while (true) {
        // Asks for input and try to connect to the server.
        getInput(playerName, serverName, port, window);
        // Connect to server
        portNumber = atoi(port);
        // AF_INET address family that is used to designate the type
        // of addresses. (IP v4 addresses)
        // SOCK_STREAM Provides sequenced, reliable, bidirectional, connection-mode byte streams, and may provide a
        // transmission mechanism for out-of-band data.
        // Default protocol.
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

void getInput(char *name, char *serverName, char *port, WINDOW * window) {
    bzero(name, MAXIMUM_INPUT_STRING);
    bzero(serverName, MAXIMUM_INPUT_STRING);
    bzero(port, MAXIMUM_INPUT_STRING);
    // Display Input name Window
    for (int i = 0; i < PLAY_GAME_MENU_ITEMS; i++) {
        mvwprintw(window, i, 0, SERVER_REQUIRED[i]);
    }

    wrefresh(window);
    // Enable user to write on screen
    echo();
    // Get name
    wmove(window, 0, PLAY_GAME_MENU_LENGTH);
    wgetstr(window, name);
    name[MAXIMUM_INPUT_STRING - 1] = '\0';
    // Get Server name
    wmove(window, 1, PLAY_GAME_MENU_LENGTH);
    wgetstr(window, serverName);
    // Get port number
    wmove(window, 2, PLAY_GAME_MENU_LENGTH);
    wgetstr(window, port);
    clearWindow(window);
}

bool printErrorAndOfferRetry(char *errorMessage) {
    WINDOW * window = createWindowAtTheCenterOfTheScreen(2, 20);

    mvwprintw(window, 0, 0, errorMessage);
    mvwprintw(window, 1, 0, "Retry? (Y/n)");
    wrefresh(window);
    int retry = getch();
    if (retry == 'Y' || retry == 'y') {
        clearWindow(window);
        delwin(window);
        return true;
    }
    clearWindow(window);
    delwin(window);
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

Vector * readConnectionsFromSocket(int socketFileDescriptor) {
    int response;
    int amountOfConnections;
    size_t connectionsSize;
    unsigned char bufferInteger[INTEGER_BYTES];
    bzero(bufferInteger, INTEGER_BYTES);
    Vector * connections = initVector();

    response = (int) read(socketFileDescriptor, bufferInteger, INTEGER_BYTES);

    if (response == -1) {
        perror("Error when reading from socket");
        close(socketFileDescriptor);
        return NULL;
    }

    deserializeInt(bufferInteger, &amountOfConnections);
    connectionsSize = (size_t) (amountOfConnections * CONNECTION_BYTES_NO_SNAKE);

    unsigned char buffer[connectionsSize];
    bzero(buffer, connectionsSize);

    response = (int) read(socketFileDescriptor, buffer, connectionsSize);

    if (response == -1) {
        perror("Error when reading from socket");
        close(socketFileDescriptor);
        exit(1);
    }
    // TODO: more efficient pls.
    // De-serialize the connections and put them in a vector
    deserializeVectorOfConnections(buffer, connections, amountOfConnections);
    return connections;
}

void waitUntilHostStartsGame(WINDOW *window, int *sockFd, char * playerId) {
    Vector * connections = NULL;
    bool isHost = false;
    // Set getCh delay.
    halfdelay(WAIT_INPUT_TIME_FOR_HOST_TO_START_GAME);

    while (true) {

        int nextAction = readDelimiterQueue(sockFd);

        if (nextAction == 1) {
            clearWindow(window);
            delwin(window);
            gameInit(connections, *sockFd);
            break;
        } else if (nextAction == 2) {
            if (connections != NULL) {
                clearConnectionVector(connections);
            }
            // Get connections the server has
            connections = readConnectionsFromSocket(*sockFd);
            isHost = checkIfHost(connections, playerId);
            clearWindow(window);
            generateWindowForWaitingInQueue(connections, window, isHost);
        } else if (isHost) {
            noecho();
            int input = getch();
            // Start Game
            if (input == 'S') {
                writeStartGameToSocket(sockFd);
                // Remove delay from char.
                cbreak();
                clearWindow(window);
                delwin(window);
                gameInit(connections, *sockFd);
                break;
            }
        } else {
            sleep(SLEEP_WHEN_NO_HOST_QUEUE_SEC);
        }
    }
}

void writeStartGameToSocket(int *sockFd) {
    int response;
    unsigned char buffer[DELIMITERS_SIZE];
    bzero(buffer, DELIMITERS_SIZE);

    serializeCharArray(buffer, HOST_STARTS_GAME_DELIMITER, DELIMITERS_SIZE);
    response = (int) write(*sockFd, buffer, DELIMITERS_SIZE);

    if (response == -1) {
        perror("Error reading from socket");
        close(*sockFd);
        exit(1);
    }
}

int readDelimiterQueue(int *sockFd) {
    int response;
    unsigned char buffer[DELIMITERS_SIZE];

    bzero(buffer, DELIMITERS_SIZE);

    setSocketBlockingEnabled(*sockFd, false);
    response = (int) read(*sockFd, buffer, DELIMITERS_SIZE);
    setSocketBlockingEnabled(*sockFd, true);

    if (response < 0) {
        return -1;
    }

    if (strncmp((const char *) buffer, HOST_STARTS_GAME_DELIMITER, DELIMITERS_SIZE) == 0) {
        return 1;
    } else if (strncmp((const char *) buffer, VECTOR_OF_CONNECTIONS_DELIMITER, DELIMITERS_SIZE) == 0) {
        return 2;
    } else {
        return -2;
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
