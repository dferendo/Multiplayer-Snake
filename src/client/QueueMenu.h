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

void clearConnectionVector(Vector * oldVector);

Vector * readConnectionsFromSocket(int socketFileDescriptor);

bool printErrorAndOfferRetry(char *errorMessage);

void waitUntilHostStartsGame(WINDOW * window, int * sockFd, char * playerId);

void writeStartGameToSocket(int * sockFd);

bool readStartGameFromSocket(int * sockFd);

#endif //SNAKES_STARTGAME_H
