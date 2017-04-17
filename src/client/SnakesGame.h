//
// Created by dylan on 17/04/2017.
//

#ifndef SNAKES_SNAKESGAME_H
#define SNAKES_SNAKESGAME_H

#include "../utility/Vector.h"

void gameInit(Vector * connections);

void sendAllSnakeDataToAllConnections(Vector * connections);

#endif //SNAKES_SNAKESGAME_H
