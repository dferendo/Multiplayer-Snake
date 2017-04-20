//
// Created by dylan on 14/04/2017.
//

#ifndef SNAKES_CLIENTLAYOUT_H
#define SNAKES_CLIENTLAYOUT_H

#include <ncurses.h>
#include "../utility/Vector.h"
#include "../server/Food.h"

// Put 0 for clean border
#define MAIN_MENU_BORDER_CHARACTER '+'
#define MAIN_MENU_DIVIDER '-'
#define MAIN_MENU_ITEMS 3
#define PLAY_GAME_MENU_REQUIRED 3
#define PLAY_GAME_MENU_LENGTH 13

#define ERROR_NO_HOST "Error, no such host!"
#define ERROR_CONNECTION_FAILED "Error, connection failed!"

// Wait 2 seconds when using half delay since it waits n tenths
// of a second.
#define WAIT_INPUT_TIME_FOR_HOST_TO_START_GAME 20

#define HOST_GAME_START "Press 'S' to start Game!"

#define SLEEP_WHEN_NO_HOST_QUEUE_SEC 2

#define SNAKE_CHARACTER "#"

const char * const MENU_ITEMS[MAIN_MENU_ITEMS];

const char * const CREDITS;

const char * const SERVER_REQUIRED[PLAY_GAME_MENU_REQUIRED];

void createOutsideBorder();

WINDOW * createMainMenuWindow();

void aboutMenu();

void getInput(char * name, char * serverName, char * port);

void generateWindowForWaitingInQueue(Vector * connections, WINDOW * window, bool isHost);

WINDOW * generatePlayingWindow();

void deleteWindow(WINDOW *window);

WINDOW * createWindowAtTheCenterOfTheScreen(int height, int width);

void showWinnerScreen();

void showDeadScreen();

const chtype foodType(Food * type);

#endif //SNAKES_CLIENTLAYOUT_H
