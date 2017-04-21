//
// Created by dylan on 14/04/2017.
//

#ifndef SNAKES_STARTGAME_H
#define SNAKES_STARTGAME_H

#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <ncurses.h>
#include "../server/Server.h"

void serverConnection(int portNumber, char * hostName);

bool connectToServer(int * sockFd, int portNumber, char * hostName);

#endif //SNAKES_STARTGAME_H
