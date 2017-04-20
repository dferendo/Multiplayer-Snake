//
// Created by dylan on 20/04/2017.
//

#ifndef SNAKES_QUEUEAPI_H
#define SNAKES_QUEUEAPI_H

void writeNameToSocket(int socketFileDescriptor, char * name);

void writeStartGameToSocket(int * sockFd);

int readDelimiterQueue(int *sockFd);

Vector * readConnectionsFromSocket(int socketFileDescriptor);

#endif //SNAKES_QUEUEAPI_H
