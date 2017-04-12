//
// Created by dylan on 11/04/2017.
//

#include <ncurses.h>

#ifndef SNAKES_GAMEBOARD_H
#define SNAKES_GAMEBOARD_H

// Since this is a multiple game, the game board need to be fixed
// otherwise it would be unfair for other players.
#define ROW 24
#define COLUMN 89

void createBorder();

WINDOW * createSnakeRoamingWindow();

void clearWindow(WINDOW * window);

WINDOW * resetWindow(WINDOW * window);

#endif //SNAKES_GAMEBOARD_H
