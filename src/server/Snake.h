//
// Created by dylan on 11/04/2017.
//

#ifndef SNAKES_SNAKE_H
#define SNAKES_SNAKE_H

#include <ncurses.h>
#include "../utility/LinkedList.h"
#include "../utility/Vector.h"

// Mark the opposite direction give the direction
#define OPPOSITE_DIR(x) x == D_UP ? D_DOWN : x == D_LEFT ? D_RIGHT : x == D_DOWN ? D_UP : x == D_RIGHT ? D_LEFT : D_UP

/**
 * All positions the snake can go.
 */
typedef enum {
    D_UP = 0,
    D_LEFT = 1,
    D_DOWN = 2,
    D_RIGHT = 3
} Direction;

/**
 * x, y coordinates of the game board.
 */
typedef struct Position {
    int x;
    int y;
} Position;

/**
 * The snake containing the current direction, size and all positions of
 * the snake in a linked list.
 */
typedef struct Snake {
    Direction direction;
    int size;
    LinkedListPosition * positions;
} Snake;

/**
 * Creates a Snake. The size of the snake and starting direction depends on
 * {@link GameSettings.h}. Snake is created at a random location. The location
 * of the snake is unique, there is nothing on the snake tiles.
 *
 * @note: restart and currentCount are used when the game was restarted since
 * all connection will be restarted, the methods needs to know which snakes
 * were set and which there weren't.
 *
 * @param connections: All current connections.
 * @param foods: All current foods.
 * @param restart: True if the game was restarted, false otherwise.
 * @param currentCount: The current snakes created, used when restart is true.
 * @return A snake with all the default settings at a random location, NULL
 * if an error was encountered.
 */
Snake * createSnake(Vector * connections, Vector * foods, bool restart, int currentCount);

/**
 * Check if two locations are equal.
 *
 * @param position1: Position.
 * @param position2: Another Position.
 * @return True if two positions corresponds to the same coordinate, false otherwise.
 */
bool checkIfPositionsAreEqual(Position * position1, Position * position2);

#endif //SNAKES_SNAKE_H
