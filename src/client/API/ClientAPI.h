//
// Created by dylan on 21/04/2017.
//

#ifndef SNAKES_CLIENTAPI_H
#define SNAKES_CLIENTAPI_H

#include <stdbool.h>
#include "../../utility/Vector.h"

int readDelimiterSnakes(int socketFd);

Vector * readFoodsFromSocket(int socketFileDescriptor);

Vector * readSnakesFromSocket(int sockFd);

bool sendUserDirection(int sockFd, int direction);

#endif //SNAKES_CLIENTAPI_H
