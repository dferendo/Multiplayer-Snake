//
// Created by dylan on 14/04/2017.
//

#ifndef SNAKES_SERVERLAUNCHER_H
#define SNAKES_SERVERLAUNCHER_H

#include <stdbool.h>
#include "../utility/Vector.h"

typedef struct ClientInfo {
    char * name;
    bool isHost;
} ClientInfo;

typedef struct Connection {
    int sockFd;
    ClientInfo * clientInfo;
} Connection;

// Vector containing Connections Type
extern Vector * connections;

#endif //SNAKES_SERVERLAUNCHER_H
