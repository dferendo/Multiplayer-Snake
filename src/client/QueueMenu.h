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

void queueConnectionManager(WINDOW *window);

bool connectToServer(WINDOW * window, int * sockFd, char * playerName);

void getInput(char * name, char * serverName, char * port, WINDOW * window);

void writeNameToSocket(int socketFileDescriptor, char * name);

bool checkIfThereAreConnections(int socketFileDescriptor);

void readConnectionsFromSocket(int socketFileDescriptor, Vector * connections);

bool printErrorAndOfferRetry(char *errorMessage);

bool waitUntilHostStartsGame(WINDOW * window, int * sockFd, Vector * connections);

#endif //SNAKES_STARTGAME_H
