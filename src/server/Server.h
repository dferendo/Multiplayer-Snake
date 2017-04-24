//
// Created by dylan on 14/04/2017.
//

#ifndef SNAKES_SERVERLAUNCHER_H
#define SNAKES_SERVERLAUNCHER_H

#include <stdint.h>
#include "../utility/Vector.h"
// Lock.
extern pthread_mutex_t lock;
// Holds all current foods.
extern Vector * foods;
// Holds all current connections.
extern Vector * connections;
// Server socket.
extern int sockFd;
/**
 * Params passed to the server Thread.
 */
typedef struct ServerParams {
    uint16_t portNumber;
} ServerParams;

int main(int argc, char *argv[]);

/**
 * Starts the game pthread. This restarts every time there is a winner.
 */
void startGameThread();

/**
 * Starts the server thread. Never restarts.
 *
 * @param portNumber: port number of the server.
 */
void startServerThread(uint16_t portNumber);

/**
 * Sets all the data required for the new game. Creates the initial snakes positions
 * and send the data and foods data to the clients.
 *
 * @note There is a brief delay every time a new game starts.
 *
 * @param foods: All current Foods.
 * @param connections: All current Connections.
 */
void restartGame(Vector * foods, Vector * connections);

void terminateServer(int sig);

#endif //SNAKES_SERVERLAUNCHER_H