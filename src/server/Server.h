//
// Created by dylan on 14/04/2017.
//

#ifndef SNAKES_SERVERLAUNCHER_H
#define SNAKES_SERVERLAUNCHER_H

#include "../utility/Vector.h"
#include <stdbool.h>

typedef struct ClientInfo {
    char * name;
    short isHost;
} ClientInfo;

typedef struct Connection {
    int sockFd;
    ClientInfo * clientInfo;
} Connection;

typedef struct CreateConnectThreadArguments {
    short isHost;
    int sockFd;
} CreateConnectThreadArguments;

// Vector containing Connections Type
extern Vector * connections;

#endif //SNAKES_SERVERLAUNCHER_H
