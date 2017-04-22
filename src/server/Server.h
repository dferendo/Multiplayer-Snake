//
// Created by dylan on 14/04/2017.
//

#ifndef SNAKES_SERVERLAUNCHER_H
#define SNAKES_SERVERLAUNCHER_H

#include <stdint.h>
#include "../utility/Vector.h"
extern pthread_mutex_t lock;

typedef struct GameThreadParams {
    Vector * connections;
    Vector * foods;
} GameThreadParams;

typedef struct ServerParams {
    Vector * connections;
    Vector * foods;
    uint16_t portNumber;
} ServerParams;

int main(int argc, char *argv[]);

void startGameThread(Vector * foods, Vector * connections);

void startServerThread(Vector * foods, Vector * connections, uint16_t portNumber);

void restartGame(Vector * foods, Vector * connections);


#endif //SNAKES_SERVERLAUNCHER_H