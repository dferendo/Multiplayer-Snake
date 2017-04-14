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

void startGameInit();

void getInput(char * name, char * serverName, char * port);

bool printError(char * errorMessage);

WINDOW * createWindowAtTheCenterOfTheScreen();

bool attemptToConnectToServer(int sockFd, int portNumber,
                              struct sockaddr_in * serverAddress,
                              struct hostent * server);

#endif //SNAKES_STARTGAME_H
