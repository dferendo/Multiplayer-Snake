//
// Created by dylan on 14/04/2017.
//

#ifndef SNAKES_STARTGAME_H
#define SNAKES_STARTGAME_H

#include <stdbool.h>

/**
 * Keep track of the server and the game. If cannot connect to server, program will exit
 * and display the user the problem. It also calls the method to initialise the game and
 * restarts the game when finished.
 *
 * @param portNumber: The port of the server.
 * @param hostName: The server name.
 */
void serverManager(int portNumber, char *hostName);

/**
 * Connect to the server using TCP/IP protocol where all addresses are welcomed. Will print
 * on screen if connection failed and the type of error it was, using ncurses.
 *
 * @param sockFd: The socket File Descriptor.
 * @param portNumber: Port number used.
 * @param hostName: Server name.
 * @return Returns true if connection was successful, false otherwise.
 */
bool connectToServer(int * sockFd, int portNumber, char * hostName);

/**
 * Closes the socket and end the ncurses window. This is called whenever an interrupt is
 * requested (CTRL + C). Other resources will be cleaned by the OS
 *
 * @param sig: The type of signal.
 */
void terminateClient(int sig);

#endif //SNAKES_STARTGAME_H
