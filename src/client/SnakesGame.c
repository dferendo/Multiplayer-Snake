//
// Created by dylan on 17/04/2017.
//
#include "SnakesGame.h"
#include "../server/Server.h"
#include "../template/ClientLayout.h"
#include "../utility/Serialize.h"
#include "../utility/General.h"
#include "../server/Food.h"
#include <strings.h>
#include <unistd.h>
#include <memory.h>

void gameInit(Vector * connections, int sockFd) {
    Vector * foods = NULL;

    gameRunning(connections, foods, sockFd);
}

void gameRunning(Vector *connections, Vector * foods, int sockFd) {
    int nextCompute;
    WINDOW * window = NULL;

    while (true) {
        nextCompute = readDelimiterSnakes(sockFd);

        if (nextCompute == 1) {
            if (foods != NULL) {
                clearFoodsVector(foods);
            } else if (window != NULL) {
                clearWindow(window);
            }
            foods = readFoodsFromSocket(sockFd);
            window = displayNewData(foods, connections);
            wrefresh(window);
        }
    }
}

int readDelimiterSnakes(int socketFd) {
    int response;
    unsigned char buffer[DELIMITERS_SIZE];

    bzero(buffer, DELIMITERS_SIZE);

    // Enable non blocking for read.
    setSocketBlockingEnabled(socketFd, false);
    response = (int) read(socketFd, buffer, DELIMITERS_SIZE);
    // Disable non blocking for read.
    setSocketBlockingEnabled(socketFd, true);

    if (response < 0) {
        return -1;
    }

    if (strncmp((const char *) buffer, VECTOR_OF_FOOD_DELIMITER, DELIMITERS_SIZE) == 0) {
        return 1;
    } else {
        return -2;
    }
}

Vector *readFoodsFromSocket(int socketFileDescriptor) {
    int response, amountOfFood;
    size_t foodSize;
    // First read the amount of Food.
    unsigned char bufferInteger[INTEGER_BYTES];
    bzero(bufferInteger, INTEGER_BYTES);
    Vector * foods = initVector();

    response = (int) read(socketFileDescriptor, bufferInteger, INTEGER_BYTES);

    if (response == -1) {
        perror("Error when reading from socket");
        close(socketFileDescriptor);
        return NULL;
    }

    deserializeInt(bufferInteger, &amountOfFood);
    foodSize = (size_t) (amountOfFood * FOOD_BYTES_SIZE);

    // Now read the actual Food.
    unsigned char buffer[foodSize];
    bzero(buffer, foodSize);

    response = (int) read(socketFileDescriptor, buffer, foodSize);

    if (response == -1) {
        perror("Error when reading from socket");
        close(socketFileDescriptor);
        exit(1);
    }

    // De-serialize foods and put them in a vector
    deserializedVectorOfFoods(buffer, foods, amountOfFood);
    return foods;
}

void clearFoodsVector(Vector *foods) {
    Food * food;

    for (int i = 0; i < foods->size; i++) {
        food = (Food *) foods->data[i];
        free(food->position);
    }

    deleteVector(foods);
}

WINDOW *displayNewData(Vector *foods, Vector *connections) {
    WINDOW *window = generatePlayingWindow();
    Food * food;

    // Display Foods.
    for (int i = 0; i < foods->size; i++) {
        food = (Food *) foods->data[i];
        mvwprintw(window, food->position->y, food->position->x, "o");
    }
    return window;
}