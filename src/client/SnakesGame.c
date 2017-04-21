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

void gameInit(int sockFd) {
    pthread_t characterReaderTId;

    ReadUserInputThreadParams * inputThreadParams =
            (ReadUserInputThreadParams *) malloc(sizeof(ReadUserInputThreadParams));

    if (inputThreadParams == NULL) {
        return;
    }
    inputThreadParams->sockFd = sockFd;
    // Create a thread that reads user input.
    if (pthread_create(&characterReaderTId, NULL, readDirectionFromUser, inputThreadParams) != 0) {
        perror("Could not create a snake thread.");
        return;
    }

    gameRunning(sockFd);
}

void gameRunning(int sockFd) {
    Vector * foods = NULL;
    Vector * snakes = NULL;
    int nextCompute;
    WINDOW * window = generatePlayingWindow();

    while (true) {
        nextCompute = readDelimiterSnakes(sockFd);

        if (nextCompute == 1) {
            if (!foodHandler(sockFd, foods, snakes)) {
                // There was an error which cannot be fixed, stop game.
                break;
            }
        } else if (nextCompute == 2) {
            snakeHandler(sockFd, foods, snakes);
        } else if (nextCompute == 3) {
            deleteWindow(window);
            showWinnerScreen();
            // Stop loop
            break;
        } else if (nextCompute == 4) {
            deleteWindow(window);
            showDeadScreen();
            // Stop loop
            break;
        } else if (nextCompute == 0) {
            // There was no reading
            continue;
        } else {
            // Error has occurred.
            deleteWindow(window);
            serverErrorScreen();
            break;
        }
    }
}

void clearFoodsVector(Vector *foods) {
    Food * food;
    if (foods == NULL) {
        return;
    }

    for (int i = 0; i < foods->size; i++) {
        food = (Food *) foods->data[i];
        free(food->position);
    }

    deleteVector(foods);
}

void clearSnakeVector(Vector *snakes) {
    Snake * snake;

    if (snakes == NULL) {
        return;
    }

    for (int i = 0; i < snakes->size; i++) {
        snake = (Snake *) snakes->data[i];
        freeSnake(snake);
    }
}

bool foodHandler(int sockFd, Vector *foods, Vector * snakes) {
    clearFoodsVector(foods);
    deleteWindow(window);
    foods = readFoodsFromSocket(sockFd);
    // Error with foods
    if (foods == NULL) {
        deleteWindow(window);
        serverErrorScreen();
        return false;
    }
    window = displayNewData(foods, snakes);
    wrefresh(window);
    return true;
}

bool snakeHandler(int sockFd, Vector *foods, Vector *snakes) {
    clearSnakeVector(snakes);
    deleteWindow(window);

    snakes = readSnakesFromSocket(sockFd);
    // Error with snakes
    if (snakes == NULL) {
        deleteWindow(window);
        serverErrorScreen();
        return false;
    }
    window = displayNewData(foods, snakes);
    wrefresh(window);
    return true;
}

void *readDirectionFromUser(void *args) {
    int sockFd = ((ReadUserInputThreadParams *) args)->sockFd;
    int character, previousChar = DEFAULT_START_DIRECTION_KEY;

    while (true) {
        character = getch();
        switch (character) {
            case 'w':
                // If snake allowed to go opposite side, it would mean he is dead.
                if (previousChar != 'x') {
                    if (sendUserDirection(sockFd, D_UP)) {
                        previousChar = character;
                    } else {
                        // If you cannot write to the socket, that indicates
                        // that the server close.
                        pthread_exit(NULL);
                    }
                }
                break;
            case 'a':
                if (previousChar != 'd') {
                    if (sendUserDirection(sockFd, D_LEFT)) {
                        previousChar = character;
                    } else {
                        // If you cannot write to the socket, that indicates
                        // that the server close.
                        pthread_exit(NULL);
                    }
                }
                break;
            case 'd':
                if (previousChar != 'a') {
                    if (sendUserDirection(sockFd, D_RIGHT)) {
                        previousChar = character;
                    } else {
                        // If you cannot write to the socket, that indicates
                        // that the server close.
                        pthread_exit(NULL);
                    }
                }
                break;
            case 'x':
                if (previousChar != 'w') {
                    if (sendUserDirection(sockFd, D_DOWN)) {
                        previousChar = character;
                    } else {
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
