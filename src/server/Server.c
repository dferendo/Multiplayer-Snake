//
// Created by dylan on 14/04/2017.
//
#include "Server.h"
#include "../utility/General.h"
#include "Game.h"
#include "API/SnakesAPI.h"
#include "Food.h"
#include "../settings/GameSettings.h"
#include <pthread.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    Vector * connections;
    Vector * foods;
    // Seed
    srand((unsigned int) time(NULL));

    if (argc < 2) {
        printf("Port number was not passed");
        exit(1);
    }
    pthread_mutex_init(&lock, NULL);

    // Initialize threads
    connections = initVector();

    if (connections == NULL) {
        exit(1);
    }

    foods = initVector();

    if (foods == NULL) {
        exit(1);
    }
    // Start Server
    startServerThread(foods, connections, (uint16_t) atoi(argv[1]));

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
    // Re-declare variables
    foods = initVector();
    // Give starting positions
    for (int i = 0; i < connections->size; i++) {
        connection = (Connection *) connections->data[i];
        // Re-generate snake random position
        connection->snake = createSnake(connections, foods);
    }
    // Send data
    do {
        // Send data again, if a connection is lost re-send the data.
        error = sendSnakeDataToClients(connections);
        // Send Food data as well
        error |= writeFoodDataToClients(connections, foods);;
    } while (!error);
}

void startGameThread(Vector *foods, Vector *connections) {
    pthread_t gameThread;
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

    // Game Thread
    if (pthread_create(&gameThread, NULL, gameInitialize, gameArgs) != 0) {
        perror("Could not create a worker thread.");
        free(gameArgs);
        deleteVector(foods);
        deleteVector(connections);
        exit(1);
    }
    // Wait for thread to finish
    pthread_join(gameThread, NULL);
    free(gameArgs);
}

void startServerThread(Vector *foods, Vector *connections, uint16_t portNumber) {
    pthread_t serverThread;
    ServerParams * serverArgs = (ServerParams *) malloc(sizeof(ServerParams));

    if (serverArgs == NULL) {
        perror("Error on malloc arguments");
        deleteVector(connections);
        deleteVector(foods);
        exit(1);
    }

    serverArgs->connections = connections;
    serverArgs->foods = foods;
    serverArgs->portNumber = portNumber;

    // Server Thread
    if (pthread_create(&serverThread, NULL, serverInit, serverArgs) != 0) {
        perror("Could not create a worker thread.");
        free(serverArgs);
        deleteVector(foods);
        deleteVector(connections);
        exit(1);
    }
}
