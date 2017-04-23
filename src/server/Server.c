//
// Created by dylan on 14/04/2017.
//
#include "Server.h"
#include "../utility/General.h"
#include "Game.h"
#include "API/SnakesAPI.h"
#include "../settings/GameSettings.h"
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
Vector * foods;
Vector * connections;

int main(int argc, char *argv[]) {
    // Seed
    srand((unsigned int) time(NULL));

    if (argc < 2) {
        printf("Port number was not passed");
        exit(1);
    }
    pthread_mutex_init(&lock, NULL);
    connections = initVector();

    if (connections == NULL) {
        exit(1);
    }

    foods = initVector();

    if (foods == NULL) {
        exit(1);
    }
    // Start Server
    startServerThread((uint16_t) atoi(argv[1]));

    while (true) {
        // Start game
        startGameThread(foods, connections);
        // Whenever a game ends, it will return.
        pthread_mutex_lock(&lock);
        restartGame(foods, connections);
        pthread_mutex_unlock(&lock);
        // Give some time before starting new game.
        sleep(PROMPT_SCREEN_DELAY);
    }
}

void restartGame(Vector * foods, Vector * connections) {
    Connection * connection;
    bool error;
    // Give starting positions
    for (int i = 0; i < connections->size; i++) {
        connection = (Connection *) connections->data[i];
        // Re-generate snake random position
        connection->snake = createSnake(connections, foods, true, i);
    }
    // Send data
    do {
        // Send data again, if a connection is lost re-send the data.
        error = sendSnakeDataToClients(connections);
        error |= writeFoodDataToClients(connections, foods);
    } while (!error);
}

void startGameThread() {
    pthread_t gameThread;

    // Game Thread
    if (pthread_create(&gameThread, NULL, gameInitialize, NULL) != 0) {
        perror("Could not create a worker thread.");
        deleteVector(foods);
        deleteVector(connections);
        exit(1);
    }
    // Wait for thread to finish
    pthread_join(gameThread, NULL);
}

void startServerThread(uint16_t portNumber) {
    pthread_t serverThread;
    ServerParams * serverArgs = (ServerParams *) malloc(sizeof(ServerParams));

    if (serverArgs == NULL) {
        perror("Error on malloc arguments");
        deleteVector(connections);
        deleteVector(foods);
        exit(1);
    }

    serverArgs->portNumber = portNumber;

    // Server Thread
    if (pthread_create(&serverThread, NULL, serverInit, serverArgs) != 0) {
        perror("Could not create a worker thread.");
        free(serverArgs);
        deleteVector(foods);
        deleteVector(connections);
        exit(1);
    }
    free(serverArgs);
}
