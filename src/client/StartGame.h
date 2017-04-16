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

void startGameInit();

void getInput(char * name, char * serverName, char * port);

void writeNameToSocket(int socketFileDescriptor, char * name);

void readConnectionsFromSocket(int socketFileDescriptor, Vector * connections);

bool printErrorAndOfferRetry(char *errorMessage);

WINDOW * displayConnections(Vector * connections);

#endif //SNAKES_STARTGAME_H
