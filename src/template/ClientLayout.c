//
// Created by dylan on 15/04/2017.
//
#include "ClientLayout.h"
#include "GameSettings.h"
#include "../server/Server.h"
#include "unistd.h"

void generateWindowForWaitingInQueue(Vector * connections, WINDOW * window, bool isHost) {

    mvwprintw(window, 0, 0, "Players waiting: ");
    for (int i = 0; i < connections->size; i++) {
        char str[MAXIMUM_INPUT_STRING + 10];
        Connection * nextConnection = (Connection *) connections->data[i];

        if (nextConnection->clientInfo->isHost) {
            sprintf(str, "%d) %s (H)", i + 1, nextConnection->clientInfo->name);
        } else {
            sprintf(str, "%d) %s", i + 1, nextConnection->clientInfo->name);
        }
        mvwprintw(window, i + 1, 0, str);
    }
    if (isHost) {
        mvwprintw(window, (int) (connections->size + 2), 0, HOST_GAME_START);
    }
    wrefresh(window);
}

WINDOW *generatePlayingWindow() {
    WINDOW * window;
    // Creating playing window without borders.
    window = newwin(MAIN_WINDOW_ROW - 2, MAIN_WINDOW_COLUMN - 2, 1, 1);
    return window;
}

void clearWindow(WINDOW *window) {
    wclear(window);
    wrefresh(window);
}

WINDOW * createWindowAtTheCenterOfTheScreen(int height, int width) {
    WINDOW * tempWindow;
    // StartingX and Y put in centre (Approx)
    int startingX = (MAIN_WINDOW_COLUMN / 2) - (width / 2), startingY = (MAIN_WINDOW_ROW / 2) - height;
    // Create new window where main menu will be placed.
    tempWindow = newwin(height, width, startingY, startingX);
    return tempWindow;
}

void showWinnerScreen() {
    WINDOW * tempWindow = createWindowAtTheCenterOfTheScreen(1, 10);
    mvwprintw(tempWindow, 0, 0, "You Win!!");
    wrefresh(tempWindow);
    sleep(DEAD_WIN_SCREEN_DELAY_SEC);
    clearWindow(tempWindow);
    delwin(tempWindow);
}

void showDeadScreen() {
    WINDOW * tempWindow = createWindowAtTheCenterOfTheScreen(1, 10);
    mvwprintw(tempWindow, 0, 0, "You Died");
    wrefresh(tempWindow);
    sleep(DEAD_WIN_SCREEN_DELAY_SEC);
    clearWindow(tempWindow);
    delwin(tempWindow);
}
