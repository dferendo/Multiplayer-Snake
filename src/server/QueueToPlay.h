//
// Created by dylan on 15/04/2017.
//

#ifndef SNAKES_QUEUETOPLAY_H
#define SNAKES_QUEUETOPLAY_H

#include "../utility/Vector.h"
#include "Server.h"

void * initNewConnection(void *arg);

Connection * createConnection(short isHost, char * name, int socketFileDescriptor);

void readNameFromSocket(int socketFileDescriptor, char * name);

void writeConnectionsToSockets(Vector *connections);

void readStartGameFromHost(int socketFileDescriptor);

void writeStartingGameToConnectionsExceptHost(Vector * connections);

#endif //SNAKES_QUEUETOPLAY_H
