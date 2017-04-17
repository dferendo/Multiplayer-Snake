//
// Created by dylan on 15/04/2017.
//

#ifndef SNAKES_GENERAL_H
#define SNAKES_GENERAL_H

#include <ncurses.h>
#include "Vector.h"
#include <fcntl.h>

WINDOW * createWindowAtTheCenterOfTheScreen(int height, int width);

bool checkIfHost(Vector * connections, char * playerID);

/**
 * Returns true on success, or false if there was an error.
 * Credits to http://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking
 * for the help.
 */
bool setSocketBlockingEnabled(int sockFd, bool blocking);

#endif //SNAKES_GENERAL_H
