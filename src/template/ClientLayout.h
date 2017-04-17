//
// Created by dylan on 14/04/2017.
//

#ifndef SNAKES_CLIENTLAYOUT_H
#define SNAKES_CLIENTLAYOUT_H

#include <ncurses.h>
#include "../utility/Vector.h"

#define MAIN_MENU_BORDER_CHARACTER '+'
#define MAIN_MENU_DIVIDER '-'
#define MAIN_MENU_ITEMS 3
#define PLAY_GAME_MENU_ITEMS 3
#define PLAY_GAME_MENU_LENGTH 13

#define ERROR_NO_HOST "Error, no such host!"
#define ERROR_CONNECTION_FAILED "Error, connection failed!"

// wait 2 seconds when using half delay since it waits n tenths
// of a second.
#define WAIT_INPUT_TIME_FOR_HOST_TO_START_GAME 20

#define HOST_GAME_START "Press 'S' to start Game!"

#define SLEEP_WHEN_NO_HOST_QUEUE_SEC 1

extern const char * const MENU_ITEMS[MAIN_MENU_ITEMS];

extern const char * const CREDITS;

extern const char * const SERVER_REQUIRED[PLAY_GAME_MENU_ITEMS];

void generateWindowForWaitingInQueue(Vector * connections, WINDOW * window, bool isHost);

WINDOW * generatePlayingWindow();

void clearWindow(WINDOW *window);

#endif //SNAKES_CLIENTLAYOUT_H
