//
// Created by dylan on 14/04/2017.
//

#ifndef SNAKES_SERVERLAUNCHER_H
#define SNAKES_SERVERLAUNCHER_H

#include <stdint.h>
#include "../utility/Vector.h"

typedef struct GameThreadParams {
    Vector * connections;
    Vector * foods;
    pthread_mutex_t lock;
} GameThreadParams;

typedef struct ServerParams {
    Vector * connections;
    Vector * foods;
    uint16_t portNumber;
    pthread_mutex_t lock;
} ServerParams;

int main(int argc, char *argv[]);


#endif //SNAKES_SERVERLAUNCHER_H