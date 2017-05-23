//
// Created by dylan on 21/04/2017.
//
#include "ClientAPI.h"
#include "../../utility/Serialize.h"
#include "../../utility/General.h"
#include "../SnakesGame.h"
#include <unistd.h>
#include <memory.h>

int readDelimiterSnakes(int socketFd) {
    int response;
    unsigned char buffer[DELIMITERS_SIZE];

    bzero(buffer, DELIMITERS_SIZE);

    response = (int) read(socketFd, buffer, DELIMITERS_SIZE);

    if (response < 0) {
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
    } else if (strncmp((const char *) buffer, UNIQUE_ID_DELIMITER, DELIMITERS_SIZE) == 0) {
        return 6;
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

    if (foods == NULL) {
        exit(1);
    }

    response = (int) read(socketFileDescriptor, bufferInteger, INTEGER_BYTES);

    if (response == -1) {
        perror("Client Disconnected. Error when reading from socket");
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
        perror("Client Disconnected. Error when reading from socket");
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
    SnakeInfo * snakeInfo;
    int response, amountOfSnakes, sizeOfSnake, snakeID;
    // Reading the amount of snakes.
    size_t integerRead = INTEGER_BYTES, sizeForPositionsOfSnake;
    unsigned char bufferForReadingInt[integerRead];

    if (snakes == NULL) {
        exit(1);
    }

    bzero(bufferForReadingInt, integerRead);
    response = (int) read(sockFd, bufferForReadingInt, integerRead);

    if (response < 0) {
        perror("Client Disconnected. Failed to read from socket");
        clearSnakeVector(snakes);
        return NULL;
    }

    // Get the amount of snakes.
    deserializeInt(bufferForReadingInt, &amountOfSnakes);
    // Now get the actual snake.
    for (int i = 0; i < amountOfSnakes; i++) {
        bzero(bufferForReadingInt, integerRead);
        // First read user ID
        response = (int) read(sockFd, bufferForReadingInt, integerRead);

        deserializeInt(bufferForReadingInt, &snakeID);

        if (response < 0) {
            perror("Client Disconnected. Failed to read from socket");
            clearSnakeVector(snakes);
            return NULL;
        }
        // Now read the size of snake
        bzero(bufferForReadingInt, integerRead);

        response = (int) read(sockFd, bufferForReadingInt, integerRead);

        if (response < 0) {
            perror("Client Disconnected. Failed to read from socket");
            clearSnakeVector(snakes);
            return NULL;
        }
        deserializeInt(bufferForReadingInt, &sizeOfSnake);

        // Int contains the direction each snake size contains a position.
        sizeForPositionsOfSnake = (size_t) ((sizeOfSnake * POSITION_BYTES) + INTEGER_BYTES);
        unsigned char bufferSnake[sizeForPositionsOfSnake];
        bzero(bufferSnake, sizeForPositionsOfSnake);

        // Read the positions for the snake.
        response = (int) read(sockFd, bufferSnake, sizeForPositionsOfSnake);

        if (response < 0) {
            perror("Client Disconnected. Failed to read from socket");
            clearSnakeVector(snakes);
            return NULL;
        }
        // Malloc snake
        snake = (Snake *) malloc(sizeof(Snake));

        if (snake == NULL) {
            perror("Client Disconnected. Failed to allocate memory to snake");
            clearSnakeVector(snakes);
            return NULL;
        }

        // Put the new snake to the data.
        deserializedSnake(bufferSnake, snake, sizeOfSnake);

        snakeInfo = (SnakeInfo *) malloc(sizeof(SnakeInfo));

        if (snakeInfo == NULL) {
            perror("Client Disconnected. Failed to allocate memory to snake info");
            clearSnakeVector(snakes);
            return NULL;
        }

        snakeInfo->uniqueID = snakeID;
        snakeInfo->snake = snake;

        // Add snake to the vector
        if (addItemToVector(snakes, snakeInfo) < 0) {
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
        // Do not close socket, it will be handled by the main thread.
        return false;
    }
    return true;
}

int readUserID(int sockFd) {
    int response, uniqueID;
    unsigned char bufferInteger[INTEGER_BYTES];
    bzero(bufferInteger, INTEGER_BYTES);

    response = (int) read(sockFd, bufferInteger, INTEGER_BYTES);

    if (response < 0) {
        perror("Client Disconnected. Failed to read from socket");
        return -1;
    }

    deserializeInt(bufferInteger, &uniqueID);
    return uniqueID;
}
