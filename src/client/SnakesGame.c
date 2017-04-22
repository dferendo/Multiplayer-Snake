//
// Created by dylan on 17/04/2017.
//
#include "SnakesGame.h"
#include "template/ClientLayout.h"
#include "../settings/GameSettings.h"
#include "API/ClientAPI.h"
#include "../utility/General.h"
#include <pthread.h>
#include <unistd.h>

WINDOW * window;
Vector * foods = NULL;
Vector * snakes = NULL;

void gameManager(int sockFd) {
    int gameStatus;
    pthread_t characterReaderTId;
    bool * keepAlive = (bool *) malloc(sizeof(bool));
    
    if (keepAlive == NULL) {
        perror("Failed to allocate memory to Params");
        return;
    }
    *keepAlive = true;
    ReadUserInputThreadParams * inputThreadParams =
            (ReadUserInputThreadParams *) malloc(sizeof(ReadUserInputThreadParams));

    if (inputThreadParams == NULL) {
        perror("Failed to allocate memory to Params");
        return;
    }
    inputThreadParams->sockFd = sockFd;
    inputThreadParams->keepAlive = keepAlive;
    // Create a thread that reads user input.
    if (pthread_create(&characterReaderTId, NULL, readDirectionFromUser, inputThreadParams) != 0) {
        perror("Could not create a snake thread.");
        return;
    }

    while (true) {
        gameStatus = gameRunning(sockFd);

        if (gameStatus == 1) {
            showScreenInCentre(WINNER_TEXT);
        } else if (gameStatus == 2) {
            showScreenInCentre(DIED_TEXT);
            // Exit game if dead
            break;
        } else if (gameStatus == 3) {
            showScreenInCentre(RESTART_TEXT);
        } else if (gameStatus == -1) {
            showScreenInCentre(ERROR_CONNECTION_FAILED);
            // Exit game if server cannot be reached
            break;
        } else {
            perror("Expected error, problem with parallelism.");
            break;
        }
    }
    // If this is reached, terminate everything
    closeClient(snakes, foods, sockFd, keepAlive, characterReaderTId);
}

int gameRunning(int sockFd) {
    int nextCompute;
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
            case 0:
                // There was no reading
                continue;
            case -1:
                deleteWindow(window);
                // Connections error
                return -1;
            default:
                deleteWindow(window);
                // Unexpected data received
                return -2;
        }
    }
}

bool foodHandler(int sockFd) {
    clearFoodsVector(foods);
    deleteWindow(window);
    foods = readFoodsFromSocket(sockFd);
    // Error with foods
    if (foods == NULL) {
        deleteWindow(window);
        return false;
    }
    window = displayNewData(foods, snakes);
    wrefresh(window);
    return true;
}

bool snakeHandler(int sockFd) {
    clearSnakeVector(snakes);
    deleteWindow(window);

    snakes = readSnakesFromSocket(sockFd);
    // Error with snakes
    if (snakes == NULL) {
        deleteWindow(window);
        showScreenInCentre(ERROR_CONNECTION_FAILED);
        return false;
    }
    window = displayNewData(foods, snakes);
    wrefresh(window);
    return true;
}

void *readDirectionFromUser(void *args) {
    int sockFd = ((ReadUserInputThreadParams *) args)->sockFd;
    bool * keepAlive = ((ReadUserInputThreadParams *) args)->keepAlive;
    int character, previousChar = DEFAULT_START_DIRECTION_KEY;
    // Set time for getch() so that thread can catch if it is alive.
    halfdelay(HALF_DELAY_GET_CHAR);

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
                        free(args);
                        // If you cannot write to the socket, that indicates
                        // that the server close.
                        pthread_exit(NULL);
                    }
                }
                break;
            case 'a':
                if (previousChar != 'd' && previousChar != 'a') {
                    if (sendUserDirection(sockFd, D_LEFT)) {
                        previousChar = character;
                    } else {
                        free(args);
                        // If you cannot write to the socket, that indicates
                        // that the server close.
                        pthread_exit(NULL);
                    }
                }
                break;
            case 'd':
                if (previousChar != 'a' && previousChar != 'd') {
                    if (sendUserDirection(sockFd, D_RIGHT)) {
                        previousChar = character;
                    } else {
                        free(args);
                        // If you cannot write to the socket, that indicates
                        // that the server close.
                        pthread_exit(NULL);
                    }
                }
                break;
            case 'x':
                if (previousChar != 'w' && previousChar != 'x') {
                    if (sendUserDirection(sockFd, D_DOWN)) {
                        previousChar = character;
                    } else {
                        free(args);
                        // If you cannot write to the socket, that indicates
                        // that the server close.
                        pthread_exit(NULL);
                    }
                }
            default:
                continue;
        }
    }
}

void closeClient(Vector *snakes, Vector *foods, int socketFd, bool * keepAlive, pthread_t characterReaderId) {
    // End Character reader thread
    *keepAlive = false;
    // Connection was lost or winning of game. Wait for thread to stop.
    pthread_join(characterReaderId, NULL);

    free(keepAlive);
    // Close socket once.
    close(socketFd);
    // Clear Vectors
    clearSnakeVector(snakes);
    clearFoodsVector(foods);
}
