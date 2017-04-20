//
// Created by dylan on 20/04/2017.
//

#ifndef SNAKES_SERVERQUEUEAPI_H
#define SNAKES_SERVERQUEUEAPI_H

#include "../../utility/Vector.h"

bool readNameFromSocket(int socketFileDescriptor, char * name);

bool writeConnectionsToSockets(Vector *connections);

void readStartGameFromHost(int socketFileDescriptor);

void writeStartingGameToConnectionsExceptHost(Vector * connections);

#endif //SNAKES_SERVERQUEUEAPI_H
