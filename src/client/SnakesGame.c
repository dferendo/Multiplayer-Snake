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
        } else if (nextCompute == 2) {
            readSnakesFromSocket(connections, sockFd);
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
    } else if (strncmp((const char *) buffer, SNAKE_DETAILS_DELIMITER, DELIMITERS_SIZE) == 0) {
        return 2;
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

WINDOW *displayNewData(Vector *foods, Vector * connections) {
    WINDOW *window = generatePlayingWindow();
    Food * food;
    LinkedListPosition * snake;

    // There could be some connections but no food yet.
    if (foods != NULL) {
        // Display Foods.
        for (int i = 0; i < foods->size; i++) {
            food = (Food *) foods->data[i];
            mvwprintw(window, food->position->y, food->position->x, "o");
        }
    }

    // Display Snakes for every connection
    for (int i = 0; i < connections->size; i++) {
        snake = ((Connection *) connections->data[i])->clientInfo->snake->positions;
        // Display snake.
        do {
            mvwprintw(window, snake->position->y, snake->position->x, SNAKE_CHARACTER);
            snake = snake->next;
        } while(snake != NULL);
    }
    return window;
}

void readSnakesFromSocket(Vector * connections, int sockFd) {
    int response, sizeOfSnake, sizeForSnake;
    // Reading name and size.
    size_t size = MAXIMUM_INPUT_STRING + INTEGER_BYTES;

    // For every connection, there should be a snake.
    for (int i = 0; i < connections->size; i++) {
        unsigned char bufferName[size], name[MAXIMUM_INPUT_STRING];
        bzero(bufferName, size);

        response = (int) read(sockFd, bufferName, size);

        if (response == -1) {
            perror("Error when reading from socket");
            close(sockFd);
            return;
        }

        deserializedNameAndSizeOfSnake(bufferName, (char *) name, &sizeOfSnake);
        // Int contains the direction each snake size contains a position.
        sizeForSnake = (sizeOfSnake * POSITION_BYTES) + INTEGER_BYTES;
        unsigned char bufferSnake[sizeForSnake];

        response = (int) read(sockFd, bufferSnake, (size_t) sizeForSnake);

        if (response == -1) {
            perror("Error when reading from socket");
            close(sockFd);
            return;
        }

        // Put the new snake to the data.
        deserializedSnake(bufferSnake, clearPreviousSnakeForNewerSnake(connections, (char *) name),
                          sizeOfSnake);
    }
}

Snake * clearPreviousSnakeForNewerSnake(Vector *connections, char *name) {
    Connection * connection;
    // Find the snake
    for (int i = 0; i < connections->size; i++) {
        connection = (Connection *) connections->data[i];

        if (strncmp(connection->clientInfo->name, name, MAXIMUM_INPUT_STRING) == 0) {
            // Malloc memory if not set beforehand
            if (connection->clientInfo->snake == NULL) {
                connection->clientInfo->snake = (Snake *) malloc(sizeof(Snake));
            } else {
                // De-allocate memory Snake
                deleteLinkedListPosition(connection->clientInfo->snake->positions);
            }
            return connection->clientInfo->snake;
        }
    }
    return NULL;
}
