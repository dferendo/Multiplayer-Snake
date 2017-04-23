//
// Created by dylan on 21/04/2017.
//
#include "ClientAPI.h"
#include "../../utility/Serialize.h"
#include "../../utility/General.h"
#include <unistd.h>
#include <memory.h>
#include <errno.h>

int readDelimiterSnakes(int socketFd) {
    int response;
    unsigned char buffer[DELIMITERS_SIZE];

    bzero(buffer, DELIMITERS_SIZE);

    response = (int) read(socketFd, buffer, DELIMITERS_SIZE);

    if (response < 0) {
        // Since non-blocking, read can return this error if data was not read.
        if (errno == EAGAIN) {
            return 0;
        }
        return -1;
    }

    if (strncmp((const char *) buffer, VECTOR_OF_FOOD_DELIMITER, DELIMITERS_SIZE) == 0) {
        return 1;
    } else if (strncmp((const char *) buffer, SNAKE_DETAILS_DELIMITER, DELIMITERS_SIZE) == 0) {
        return 2;
    } else if (strncmp((const char *) buffer, WINNER_DELIMITER, DELIMITERS_SIZE) == 0) {
        return 3;
    } else if (strncmp((const char *) buffer, LOSE_DELIMITER, DELIMITERS_SIZE) == 0) {
        return 4;
    } else if (strncmp((const char *) buffer, RESTART_DELIMITER, DELIMITERS_SIZE) == 0) {
        return 5;
    } else {
        return -2;
    }
}

Vector * readFoodsFromSocket(int socketFileDescriptor) {
    int response, amountOfFood;
    size_t foodSize;
    // First read the amount of Food.
    unsigned char bufferInteger[INTEGER_BYTES];
    bzero(bufferInteger, INTEGER_BYTES);
    Vector * foods = initVector();

    response = (int) read(socketFileDescriptor, bufferInteger, INTEGER_BYTES);

    if (response == -1) {
        perror("Error when reading from socket");
        // Deallocate foods.
        clearFoodsVector(foods);
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
        clearFoodsVector(foods);
        return NULL;
    }

    // De-serialize foods and put them in a vector
    deserializedVectorOfFoods(buffer, foods, amountOfFood);
    return foods;
}

Vector * readSnakesFromSocket(int sockFd) {
    // Init Vector
    Vector * snakes = initVector();
    Snake * snake;
    int response, amountOfSnakes, sizeOfSnake;
    // Reading the amount of snakes.
    size_t sizeForAmountOfSnakes = INTEGER_BYTES, sizeForSnakeSize = INTEGER_BYTES,
            sizeForPositionsOfSnake;
    unsigned char bufferForAmountOfSnakes[sizeForAmountOfSnakes];

    bzero(bufferForAmountOfSnakes, sizeForAmountOfSnakes);
    response = (int) read(sockFd, bufferForAmountOfSnakes, sizeForAmountOfSnakes);

    if (response < 0) {
        perror("Failed to read from socket");
        clearSnakeVector(snakes);
        return NULL;
    }

    // Get the amount of snakes.
    deserializeInt(bufferForAmountOfSnakes, &amountOfSnakes);
    // Now get the actual snake.
    for (int i = 0; i < amountOfSnakes; i++) {
        // First read the size of the snake
        unsigned char bufferSizeOfSnake[sizeForSnakeSize];
        bzero(bufferSizeOfSnake, sizeForSnakeSize);

        response = (int) read(sockFd, bufferSizeOfSnake, sizeForSnakeSize);

        if (response < 0) {
            perror("Failed to read from socket");
            clearSnakeVector(snakes);
            return NULL;
        }

        deserializeInt(bufferSizeOfSnake, &sizeOfSnake);
        // Int contains the direction each snake size contains a position.
        sizeForPositionsOfSnake = (size_t) ((sizeOfSnake * POSITION_BYTES) + INTEGER_BYTES);
        unsigned char bufferSnake[sizeForPositionsOfSnake];
        bzero(bufferSnake, sizeForPositionsOfSnake);

        // Read the positions for the snake.
        response = (int) read(sockFd, bufferSnake, sizeForPositionsOfSnake);

        if (response < 0) {
            perror("Failed to read from socket");
            clearSnakeVector(snakes);
            return NULL;
        }
        // Malloc snake
        snake = (Snake *) malloc(sizeof(Snake));

        if (snake == NULL) {
            perror("Failed to allocate memory to snake");
            clearSnakeVector(snakes);
            return NULL;
        }

        // Put the new snake to the data.
        deserializedSnake(bufferSnake, snake, sizeOfSnake);

        // Add snake to the vector
        if (addItemToVector(snakes, snake) < 0) {
            freeSnake(snake);
            clearSnakeVector(snakes);
            return NULL;
        }
    }
    return snakes;
}

bool sendUserDirection(int sockFd, int direction) {
    int response;
    size_t size = DELIMITERS_SIZE + INTEGER_BYTES;
    unsigned char buffer[size];

    serializedSnakeDirectionWithDelimiter(buffer, direction);

    response = (int) write(sockFd, buffer, size);

    if (response == -1) {
        perror("Failed to write to the socket");
        // Do not close socket, it will be handled by the main thread.
        return false;
    }
    return true;
}