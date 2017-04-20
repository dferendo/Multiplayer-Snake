//
// Created by dylan on 21/04/2017.
//

#ifndef SNAKES_SERVERHANDLE_H
#define SNAKES_SERVERHANDLE_H

#include <bits/socket.h>
#include "Server.h"
#include "Snake.h"

typedef struct Connection {
    int sockFd;
    Snake * snake;
} Connection;

typedef struct CreateConnectThreadArguments {
    int sockFd;
    Vector * connections;
    Vector * foods;
    pthread_mutex_t lock;
} CreateConnectThreadArguments;

void * serverInit(void * args);

void acceptClients(int sockFd, struct sockaddr * clientAddress, socklen_t * clientSize,
                   ServerParams * serverParams);

Connection * createConnection(int socketFileDescriptor, Vector * connections,
                              Vector * foods);

void * initNewConnection(void *arg);

#endif //SNAKES_SERVERHANDLE_H
