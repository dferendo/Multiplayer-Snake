//
// Created by dylan on 15/04/2017.
//

#ifndef SNAKES_GENERAL_H
#define SNAKES_GENERAL_H

#include "Vector.h"
#include "../server/ServerHandle.h"
#include "../server/Snake.h"

/**
 * Set the socket file descriptor to non-blocking or blocking.
 * Thanks to Credits to
 * http://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking
 * for the help.
 *
 * @param sockFd: The socket file descriptor that needs to be blocked or non-blocked.
 * @param blocking: Whether the sockFd needs to be blocked or non-blocked.
 * @return: Returns true on success, or false if there was an error.
 */
bool setSocketBlockingEnabled(int sockFd, bool blocking);

/**
 * Free all data used by a malloc of a connection and close the socket of the connection.
 * Warning Connection is not freed.
 *
 * @param connection: Connection to be freed.
 */
void freeDataOfConnection(Connection *connection);

/**
 * Free all data used by a snake.
 *
 * @param snake: snake to be free.
 */
void freeSnake(Snake * snake);

/**
 * Free all the malloc used by a vector of foods.
 *
 * @param foods: Vector to be cleared.
 */
void clearFoodsVector(Vector * foods);

/**
 * Free all the malloc used by a vector of Snakes.
 *
 * @param snakes: Vector to be cleared.
 */
void clearSnakeVector(Vector * snakes);

#endif //SNAKES_GENERAL_H
