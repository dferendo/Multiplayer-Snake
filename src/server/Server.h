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


#endif //SNAKES_SERVERLAUNCHER_H