//
// Created by dylan on 15/04/2017.
//

#ifndef SNAKES_QUEUETOPLAY_H
#define SNAKES_QUEUETOPLAY_H

#include "../utility/Vector.h"
#include "Server.h"
#include <ncurses.h>

void * initNewConnection(void *arg);

Connection * createConnection(bool isHost, char * name, int socketFileDescriptor);

char * readNameFromSocket(int socketFileDescriptor);

void writeConnectionsToSockets(Vector *connections);


#endif //SNAKES_QUEUETOPLAY_H
