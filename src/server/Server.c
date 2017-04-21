//
// Created by dylan on 14/04/2017.
//
#include "Server.h"
#include "../utility/General.h"
#include "ServerHandle.h"
#include "Game.h"
#include <pthread.h>

int main(int argc, char *argv[]) {
    Vector * connections;
    Vector * foods;
    pthread_t gameThread, serverThread;
    // Seed
    srand((unsigned int) time(NULL));

    if (argc < 2) {
        printf("Port number was not passed");
        exit(1);
    }

    // Initialize threads
    connections = initVector();

    if (connections == NULL) {
        exit(1);
    }

    foods = initVector();

    if (foods == NULL) {
        exit(1);
    }

    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);
    // Create args
    GameThreadParams * gameArgs = (GameThreadParams *) malloc(sizeof(GameThreadParams));

    if (gameArgs == NULL) {
        perror("Error on malloc arguments");
        deleteVector(connections);
        deleteVector(foods);
        exit(1);
    }
    gameArgs->connections = connections;
    gameArgs->foods = foods;
    gameArgs->lock = lock;

    ServerParams * serverArgs = (ServerParams *) malloc(sizeof(ServerParams));

    if (serverArgs == NULL) {
        perror("Error on malloc arguments");
        deleteVector(connections);
        deleteVector(foods);
        free(gameArgs);
        exit(1);
    }

    serverArgs->connections = connections;
    serverArgs->foods = foods;
    serverArgs->lock = lock;
    serverArgs->portNumber = (uint16_t) atoi(argv[2]);

    // Create Threads

    // Server Thread
    if (pthread_create(&serverThread, NULL, serverInit, serverArgs) != 0) {
        perror("Could not create a worker thread.");
        free(serverArgs);
        free(gameArgs);
        deleteVector(foods);
        deleteVector(connections);
        exit(1);
    }
    // Game Thread
    if (pthread_create(&gameThread, NULL, gameInitialize, gameArgs) != 0) {
        perror("Could not create a worker thread.");
        free(serverArgs);
        free(gameArgs);
        deleteVector(foods);
        deleteVector(connections);
        exit(1);
    }

    return 0;
}
