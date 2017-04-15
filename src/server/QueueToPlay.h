//
// Created by dylan on 15/04/2017.
//

#ifndef SNAKES_QUEUETOPLAY_H
#define SNAKES_QUEUETOPLAY_H

#include <ncurses.h>
#include "../utility/Vector.h"

void * addConnectionHost(void *arg);

void * addConnectionNotHost(void *arg);

WINDOW * generateWindow(Vector * connections);

#endif //SNAKES_QUEUETOPLAY_H
