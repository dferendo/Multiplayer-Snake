//
// Created by dylan on 17/04/2017.
//
#include "SnakesGame.h"
#include "template/ClientLayout.h"
#include "../settings/GameSettings.h"
#include "API/ClientAPI.h"
#include "../utility/General.h"
#include <pthread.h>

WINDOW * window;
Vector * foods = NULL;
Vector * snakes = NULL;

int gameManager(int sockFd) {
    int gameStatus;
    ReadUserInputThreadParams * inputThreadParams;
    pthread_t characterReaderTId;
    bool * keepAlive = (bool *) malloc(sizeof(bool));

    if (keepAlive == NULL) {
        perror("Failed to allocate memory to Params");
        return -1;
    }
    *keepAlive = true;
    inputThreadParams = (ReadUserInputThreadParams *) malloc(sizeof(ReadUserInputThreadParams));

    if (inputThreadParams == NULL) {
        perror("Failed to allocate memory to Params");
        return -1;
    }
    inputThreadParams->sockFd = sockFd;
    inputThreadParams->keepAlive = keepAlive;
    // Create a thread that reads user input.
    if (pthread_create(&characterReaderTId, NULL, readDirectionFromUser, inputThreadParams) != 0) {
        perror("Could not create a read user thread.");
        free(inputThreadParams);
        return -1;
    }
    gameStatus = handleGameDataFromServer(sockFd);
    // Removes data
    removeGameData(snakes, foods, keepAlive, characterReaderTId);
    // Close game
    return closeGame(gameStatus);
}

int handleGameDataFromServer(int sockFd) {
    int nextCompute, uniqueID = -1;
    WINDOW * window = generatePlayingWindow();

    while (true) {
        nextCompute = readDelimiterSnakes(sockFd);

        switch (nextCompute) {
            case 1:
                if (!foodHandler(sockFd)) {
                    // Connection error.
                    return -1;
                }
                break;
            case 2:
                if (!snakeHandler(sockFd)) {
                    // Connection error
                    return -1;
                }
                break;
            case 3:
                deleteWindow(window);
                // Winner
                return 1;
            case 4:
                deleteWindow(window);
                // Loser
                return 2;
            case 5:
                // Restart
                deleteWindow(window);
                return 3;
            case 6:
                // Unique ID setter
                if (uniqueID != -1) {
                    printf("Setting unique ID multiple times!");
                }
                uniqueID = readUserID(sockFd);

                if (uniqueID == -1) {
                    // Connection error
                    return -1;
                }
                continue;
            default:
                deleteWindow(window);
                // Connections error
                return -1;
        }
        deleteWindow(window);
        // Update screen
        window = displayNewData(foods, snakes, uniqueID);
        wrefresh(window);
    }
}

bool foodHandler(int sockFd) {
    clearFoodsVector(foods);
    foods = readFoodsFromSocket(sockFd);
    // Error with foods
    if (foods == NULL) {
        deleteWindow(window);
        return false;
    }
    return true;
}

bool snakeHandler(int sockFd) {
    clearSnakeVector(snakes);

    snakes = readSnakesFromSocket(sockFd);
    // Error with snakes
    if (snakes == NULL) {
        deleteWindow(window);
        return false;
    }
    wrefresh(window);
    return true;
}

void * readDirectionFromUser(void *args) {
    int sockFd = ((ReadUserInputThreadParams *) args)->sockFd;
    bool * keepAlive = ((ReadUserInputThreadParams *) args)->keepAlive;
    int character, previousChar = DEFAULT_START_DIRECTION_KEY;
    // Set time for getch() so that thread can catch if it is alive.
    halfdelay(HALF_DELAY_GET_CHAR_IN_TENTH_OF_A_SEC);

    while (true) {
        if (!(*keepAlive)) {
            // Thread no longer needed
            free(args);
            // Remove delay
            cbreak();
            pthread_exit(NULL);
        }
        character = getch();
        switch (character) {
            case 'w':
                // If snake allowed to go opposite side, it would mean he is dead.
                if (previousChar != 'x' && previousChar != 'w') {
                    if (sendUserDirection(sockFd, D_UP)) {
                        previousChar = character;
                    } else {
                        // Since there was a connection error with server, stop thread.
                        *keepAlive = false;
                    }
                }
                break;
            case 'a':
                if (previousChar != 'd' && previousChar != 'a') {
                    if (sendUserDirection(sockFd, D_LEFT)) {
                        previousChar = character;
                    } else {
                        *keepAlive = false;
                    }
                }
                break;
            case 'd':
                if (previousChar != 'a' && previousChar != 'd') {
                    if (sendUserDirection(sockFd, D_RIGHT)) {
                        previousChar = character;
                    } else {
                        *keepAlive = false;
                    }
                }
                break;
            case 'x':
                if (previousChar != 'w' && previousChar != 'x') {
                    if (sendUserDirection(sockFd, D_DOWN)) {
                        previousChar = character;
                    } else {
                        *keepAlive = false;
                    }
                }
            default:
                continue;
        }
    }
}

void removeGameData(Vector *snakes, Vector *foods, bool *keepAlive, pthread_t characterReaderId) {
    // End Character reader thread
    *keepAlive = false;
    // Connection was lost or winning of game. Wait for thread to stop.
    pthread_join(characterReaderId, NULL);

    free(keepAlive);
    // Clear Vectors
    clearSnakeVector(snakes);
    clearFoodsVector(foods);
}

int closeGame(int status) {
    // If this is reached, terminate everything
    snakes = NULL;
    foods = NULL;

    if (status == 1) {
        showScreenInCentre(WINNER_TEXT);
        return 1;
    } else if (status == 2) {
        showScreenInCentre(DIED_TEXT);
        // Exit game if dead
        return -1;
    } else if (status == 3) {
        showScreenInCentre(RESTART_TEXT);
        // Do not exit game
        return 1;
    } else {
        showScreenInCentre(ERROR_CONNECTION_FAILED);
        // Exit game if server cannot be reached
        return -1;
    }
}
