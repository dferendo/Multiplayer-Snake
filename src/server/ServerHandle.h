//
// Created by dylan on 21/04/2017.
//

#ifndef SNAKES_SERVERHANDLE_H
#define SNAKES_SERVERHANDLE_H

#include <sys/socket.h>
#include "Server.h"
#include "Snake.h"

/**
 * Holds a connection of a client.
 * sockFd: The socket File descriptor.
 * snake: The client's snake.
 */
typedef struct Connection {
    int sockFd;
    unsigned int uniqueID;
    Snake * snake;
} Connection;

/**
 * Params passed to create connection pthread and client IP for display.
 */
typedef struct CreateConnectThreadArguments {
    int sockFd;
    struct sockaddr_in * pV4Addr;
} CreateConnectThreadArguments;

/**
 * pthread used to initialise the a TCP/IP server that accepts any connection. If
 * an error occurs, the server will exit the program.
 *
 * @param args: CreateConnectThreadArguments is passed as an argument.
 * @return: Nothing.
 */
void * serverInit(void * args);

/**
 * Accepts new clients that wants to communicate with the server. Will create a
 * pthread to create the connection so that multiple connections are handled
 * by the server.
 *
 * @param sockFd: Socket File descriptor.
 * @param clientAddress: The client Address.
 * @param clientSize: The size of the client.
 */
void acceptClients(int sockFd, struct sockaddr * clientAddress, socklen_t * clientSize);

/**
 * Allocates memory to create a connection and it's snake.
 *
 * @param socketFileDescriptor: The socket File Descriptor.
 * @param connections: All connections of clients.
 * @param foods: All current Foods.
 * @return The new allocated connection, NULL if an error was encountered.
 */
Connection * createConnection(int socketFileDescriptor, Vector * connections,
                              Vector * foods);

/**
 * Thread used to handle the new incoming connection from client.
 *
 * @param arg: CreateConnectThreadArguments is passed as an argument.
 * @return: Nothing.
 */
void * initNewConnection(void *arg);

#endif //SNAKES_SERVERHANDLE_H
