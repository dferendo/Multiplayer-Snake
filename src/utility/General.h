//
// Created by dylan on 15/04/2017.
//

#ifndef SNAKES_GENERAL_H
#define SNAKES_GENERAL_H

#include <ncurses.h>
#include "Vector.h"

WINDOW * createWindowAtTheCenterOfTheScreen(int height, int width);

bool checkIfHost(Vector * connections, char * playerID);

#endif //SNAKES_GENERAL_H
