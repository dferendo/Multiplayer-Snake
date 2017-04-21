//
// Created by dylan on 15/04/2017.
//

#ifndef SNAKES_GENERAL_H
#define SNAKES_GENERAL_H

#include <ncurses.h>
#include "Vector.h"
#include "../server/Server.h"
#include "../server/ServerHandle.h"
#include <fcntl.h>

/**
 * Returns true on success, or false if there was an error.
 * Credits to http://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking
 * for the help.
 */
bool setSocketBlockingEnabled(int sockFd, bool blocking);

void freeConnection(Connection * connection);

void freeSnake(Snake * snake);

#endif //SNAKES_GENERAL_H
