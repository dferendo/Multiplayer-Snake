//
// Created by dylan on 17/04/2017.
//
#include "SnakesGame.h"
#include "template/ClientLayout.h"
#include "../settings/GameSettings.h"
#include "API/ClientAPI.h"
#include "../utility/General.h"
#include <pthread.h>

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
            if (foods != NULL) {
                clearFoodsVector(foods);
            }
            deleteWindow(window);
            foods = readFoodsFromSocket(sockFd);
            // Error with foods
            if (foods == NULL) {
                deleteWindow(window);
                serverErrorScreen();
                break;
            }
            window = displayNewData(foods, snakes);
            wrefresh(window);
        } else if (nextCompute == 2) {
            if (foods != NULL) {
                clearSnakeVector(foods);
            }

            deleteWindow(window);
            // Error with snakes
            if (!readSnakesFromSocket(snakes, sockFd)) {
                deleteWindow(window);
                serverErrorScreen();
                break;
            }
            window = displayNewData(foods, snakes);
            wrefresh(window);
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

    for (int i = 0; i < foods->size; i++) {
        food = (Food *) foods->data[i];
        free(food->position);
    }

    deleteVector(foods);
}

WINDOW *displayNewData(Vector *foods, Vector * connections) {
    WINDOW *window = generatePlayingWindow();
    Food * food;
    LinkedListPosition * snake;

    // There could be some connections but no food yet.
    if (foods != NULL) {
        // Display Foods.
        for (int i = 0; i < foods->size; i++) {
            food = (Food *) foods->data[i];
            mvwaddch(window, food->position->y, food->position->x,
                      foodType(food));
        }
    }

    // Display Snakes for every connection
    for (int i = 0; i < connections->size; i++) {
        snake = ((Snake *) connections->data[i])->positions;
        // Display snake.
        do {
            mvwprintw(window, snake->position->y, snake->position->x, SNAKE_CHARACTER);
            snake = snake->next;
        } while(snake != NULL);
    }
    return window;
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

void clearSnakeVector(Vector *snakes) {
    Snake * snake;

    for (int i = 0; i < snakes->size; i++) {
        snake = (Snake *) snakes->data[i];
        freeSnake(snake);
    }
}
