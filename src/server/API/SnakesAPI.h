//
// Created by dylan on 21/04/2017.
//

#include <stdbool.h>
#include "../../utility/Vector.h"
#include "../Server.h"
#include "../Game.h"

#ifndef SNAKES_SNAKESAPI_H
#define SNAKES_SNAKESAPI_H

bool sendSnakeDataToClients(Vector *connections);

bool sendEndGameToClients(int sockFd, SnakeStatus status);

void * checkForChangeOfDirections(void * args);

bool writeFoodDataToClients(Vector * connections, Vector * foods);

#endif //SNAKES_SNAKESAPI_H
