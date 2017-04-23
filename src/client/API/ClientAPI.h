//
// Created by dylan on 21/04/2017.
//

#ifndef SNAKES_CLIENTAPI_H
#define SNAKES_CLIENTAPI_H

#include <stdbool.h>
#include "../../utility/Vector.h"

/**
 * Read the next delimiter of the snakes and returns the appropiate number. All delimiters
 * can be found in {@link Serialize.h}. If error occurred, -1 and -2 are returned.
 *
 * @param socketFd: The socket file descriptor used to communicate with the server.
 * @return 1 Vector of foods Delimiter.
 *         2 Snakes delimiter.
 *         3 Win delimiter.
 *         4 Loss delimiter.
 *         5 Restart delimiter.
 *         -1 Connection error.
 *         -2 No delimiter found.
 */
int readDelimiterSnakes(int socketFd);

/**
 * Read and de-serialize the data received from the server.
 *
 * @param socketFileDescriptor: The socket file descriptor.
 * @return: Vector of foods if data received was correct, NULL if there was an error.
 */
Vector * readFoodsFromSocket(int socketFileDescriptor);

/**
 * Read and de-serialize the data received from the server.
 *
 * @param sockFd: The socket file descriptor.
 * @return: Vector of snakes with all the positions, NULL if there was an error.
 */
Vector * readSnakesFromSocket(int sockFd);

/**
 * Send the direction, serialized, of the snake the user wants with the delimiter.
 *
 * @param sockFd: The socket file descriptor.
 * @param direction: The direction the user wants. (w, a, d, x)
 * @return: True is write was successful, false otherwise.
 */
bool sendUserDirection(int sockFd, int direction);

#endif //SNAKES_CLIENTAPI_H
