//
// Created by dylan on 14/04/2017.
//

#ifndef SNAKES_CLIENTLAYOUT_H
#define SNAKES_CLIENTLAYOUT_H

#include <ncurses.h>
#include "../../utility/Vector.h"
#include "../../server/Food.h"

// Put 0 for clean border
#define MAIN_MENU_BORDER_CHARACTER '+'
#define SNAKE_CHARACTER "#"

#define ERROR_NO_HOST "Error, no such host!"
#define ERROR_CONNECTION_FAILED "Error, connection failed!"

#define WINNER_TEXT "You Win!!"
#define DIED_TEXT "You Died"
#define RESTART_TEXT "Another snake won, restarting"

void createOutsideBorder();

void ncursesInit();

bool printError(char *errorMessage);

WINDOW * generatePlayingWindow();

void deleteWindow(WINDOW *window);

WINDOW * createWindowAtTheCenterOfTheScreen(int height);

void showScreenInCentre(char * text);

const chtype foodType(Food * type);

WINDOW * displayNewData(Vector * foods, Vector * connections);


#endif //SNAKES_CLIENTLAYOUT_H
