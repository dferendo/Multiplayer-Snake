#include "ClientLayout.h"
#include "GameSettings.h"
#include "../server/Server.h"
#include "unistd.h"

//
// Created by dylan on 15/04/2017.
//

void createOutsideBorder() {
    // Create border
    wborder(stdscr,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER);
}

WINDOW * createMainMenuWindow() {
    int windowStartingX = MAIN_WINDOW_COLUMN / 4, windowStartingY = MAIN_WINDOW_ROW / 4,
            height = MAIN_WINDOW_ROW / 2, width = MAIN_WINDOW_COLUMN / 2;

    WINDOW * menuWindow;
    // Create new window where main menu will be placed.
    menuWindow = newwin(height, width, windowStartingY, windowStartingX);
    // Draw border
    wborder(menuWindow,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER);

    // Add Divisor line.
    for(int x = 1; x < width - 1; x++) {
        mvwaddch(menuWindow, height - 3, x, MAIN_MENU_DIVIDER);
    }
    // Print menu items.
    for (int i = 0; i < MAIN_MENU_ITEMS; i++) {
        mvwprintw(menuWindow, 1 + i, 2, MENU_ITEMS[i]);
    }
    // Add Credits
    mvwprintw(menuWindow, height - 2, 2, CREDITS);
    return menuWindow;
}

void aboutMenu() {
    int windowStartingX = MAIN_WINDOW_COLUMN / 4, windowStartingY = MAIN_WINDOW_ROW / 4,
            height = MAIN_WINDOW_ROW / 2, width = MAIN_WINDOW_COLUMN / 2;

    WINDOW * menuWindow;
    // Create new window where main menu will be placed.
    menuWindow = newwin(height, width, windowStartingY, windowStartingX);
    // Draw border
    wborder(menuWindow,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER);

    mvwprintw(menuWindow, 1, 2, "A simple multiple snakes made using ncu-");
    mvwprintw(menuWindow, 2, 2, "rses library. Controls: W A D X for the ");
    mvwprintw(menuWindow, 3, 2, "respective direction of the snake. To p-");
    mvwprintw(menuWindow, 4, 2, "lay a SERVER is required where the host ");
    mvwprintw(menuWindow, 5, 2, "can start a game.");
    mvwprintw(menuWindow, height - 2, 2, "Press any key to continue");

    wrefresh(menuWindow);
    getch();
    delwin(menuWindow);
}

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
    window = newwin(MAIN_WINDOW_ROW - 1, MAIN_WINDOW_COLUMN - 1, 1, 1);
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

const chtype foodType(Food * type) {
    chtype foodTypes[] = {ACS_DIAMOND};
    return (const chtype) foodTypes[type->foodType];
}
