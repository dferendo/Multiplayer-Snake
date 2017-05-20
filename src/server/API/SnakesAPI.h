//
// Created by dylan on 21/04/2017.
//

#include <stdbool.h>
#include "../../utility/Vector.h"
#include "../Server.h"
#include "../Game.h"

#ifndef SNAKES_SNAKESAPI_H
#define SNAKES_SNAKESAPI_H

/**
 * Send the current snake positions of all snakes to all the current connections.
 * Note that if a connection cannot be reached, the connection will be terminated.
 *
 * @param connections: The connections having the socketFd to communicate and
 * containing the snake data.
 * @return: True if the send was successful, false otherwise. If it is false,
 * the connection that caused problems will be deleted.
 */
bool sendSnakeDataToClients(Vector *connections);

/**
 * Send the status of the snake to the client as a delimiter. Status send can
 * be winner, died or restart. Normal is ignore.
 *
 * @param sockFd: The socket file descriptor.
 * @param status: The status of the snake. Winner, died or restart accepted.
 * @return: True if the send was successful, false otherwise.
 *
 * @warning: If false returned, the connection is not correct and should be deleted.
 */
bool sendEndGameToClient(int sockFd, SnakeStatus status);

/**
 * A pthread that periodically checks if any of the connection clients changed the
 * direction of the snake. The read function is set to non-blocking so that
 * several clients can be serviced with a single thread. Can be terminated by
 * setting the passed boolean variable in the argument.
 *
 * @param args: ChangeDirectionParams is passed.
 * @return: Nothing
 */
void * checkForChangeOfDirections(void * args);

/**
 * Write to all the connections the current food positions.
 *
 * @param connections: All connections.
 * @param foods: All foods.
 * @return: True is the writing was successful, false otherwise.
 *
 * @warning: If false is return, the connection that failed will be
 * removed from connections.
 */
bool writeFoodDataToClients(Vector * connections, Vector * foods);

/**
 * Write the unique ID to the newly connected user.
 *
 * @param uniqueID The user unique ID
 * @return True is the writing was successful, false otherwise.
 *
 * @warning: If false is return, the connection that failed will be
 * removed from connections.
 */
bool writeUserUniqueID(int sockFd, int uniqueID);

#endif //SNAKES_SNAKESAPI_H
