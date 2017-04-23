//
// Created by dylan on 12/04/2017.
//

#ifndef SNAKES_RANDOMUTILITY_H
#define SNAKES_RANDOMUTILITY_H

#include "../server/Snake.h"

/**
 * Randomly creates the initial starting position of the snake where there is no other
 * item on that position. It also checks that the default starting size can be satisfied
 * with that position.
 *
 * @param connections: Connections containing all the snakes in the game.
 * @param foods: All the food currently in the game.
 * @return: The position where no other snake/food is on, NULL if an error occurred.
 */
Position * createInitialSnakeRandomPosition(Vector * connections, Vector * foods);

/**
 * Randomly creates the initial starting position of the snake where there is no other item
 * on that position. With every restart, all connections loses their snakes. Thus the snake
 * that is being generated needs only to check with the snakes already created, indicated
 * by the snakesCreated parameter to check if the new position is taken or not.
 *
 * @param connections: Connections containing data of snakes.
 * @param foods: All food currently in the game.
 * @param snakeCreated: The amount of snakes currently created.
 * @return: The position where no other snake/food is on, NULL if an error occurred.
 */
Position * createInitialSnakeRandomPositionForRestart(Vector * connections, Vector * foods,
                                                      int snakeCreated);

/**
 * Create a new food position. There is no other item on the new position created.
 *
 * @param positionsOfSnakes: Vector containing all snakes positions.
 * @param foodLocations: All foods location.
 * @return: The position of the food, NULL is an error occurred.
 */
Position * createFoodPosition(Vector * positionsOfSnakes, Vector * foodLocations);

#endif //SNAKES_RANDOMUTILITY_H
