//
// Created by dylan on 12/04/2017.
//

#ifndef SNAKES_RANDOMUTILITY_H
#define SNAKES_RANDOMUTILITY_H

#include "../server/Snake.h"

Position * createInitialSnakeRandomPosition(Vector * connections, Vector * foods);

Position * createFoodPosition(Vector * positionsOfSnakes, Vector * foodLocations);

#endif //SNAKES_RANDOMUTILITY_H
