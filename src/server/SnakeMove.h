//
// Created by dylan on 19/04/2017.
//

#ifndef SNAKES_SNAKEMOVE_H
#define SNAKES_SNAKEMOVE_H

#include "Snake.h"
#include "Game.h"

/**
 * Creates a temporary positions that moves the given position to its next
 * location.
 *
 * @param direction: The direction where the position will move.
 * @param position: The current position.
 * @return: The new position, NULL if an error was encountered.
 *
 * @note: The new positions will not effect the passed position.
 */
Position * moveHeadSnake(Direction direction, Position *position);

/**
 * Check all possible combinations the snake can have and moves the snake. If two snakes collide,
 * both snakes will die. If a snake next position is the tail of another snake
 * position, the snake will not die. If the snake eats a food, he will grow.
 * If the collides with the border, another snake or itself, the snake will
 * die.
 *
 * @param snake: The snake that will be moved.
 * @param foods: All the current foods.
 * @param connections: All other clients snakes.
 * @return
 */
SnakeStatus snakeAction(Snake * snake, Vector * foods, Vector * connections);

/**
 * Checks if the next position points to the next position of the snake. If
 * so change the direction to point the opposite direction. This is done to avoid
 * snake hitting himself by pressing 2 keys after each other immediately
 * resulting in the snake dying.
 * @param snake The snake to be checked.
 */
void checkIfSnakeHeadPointsToTheNextSnakePosition(Snake * snake);

/**
 * Check for a head collision. If two snakes hit each other, both snakes will die.
 *
 * @param snake: The snake that will be compared to other snakes.
 * @param connections: All the snakes.
 * @return True if there is a head collision, false otherwise.
 */
bool checkHeadCollision(Snake * snake, Vector * connections);

/**
 * Checks if the next movement is a food position. If it is, the snake will grow.
 *
 * @param snake: The selected snake that will move.
 * @param foods: All the current foods.
 * @return: True if the snake ate food, false otherwise.
 */
bool checkIfNextPositionIsFoodAndGrow(Snake *snake, Vector *foods);

/**
 * Checks if the next movement will result in a collision with the border, other snakes
 * or itself.
 *
 * @param snake: The selected snake that will move.
 * @param connections: All the connections.
 * @return True if the snake collide, false otherwise.
 */
bool checkIfNextPositionIsCollision(Snake * snake, Vector * connections);

/**
 * Moves all of the snakes positions to their respective next position.
 *
 * @param snake: The snake that will be moved.
 */
void snakeMove(Snake * snake);

#endif //SNAKES_SNAKEMOVE_H
