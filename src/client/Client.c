
#include "Client.h"
#include <stdio.h>
#include "../template/ClientLayout.h"
#include "../template/GameSettings.h"
#include "QueueMenu.h"

const char * const MENU_ITEMS[MAIN_MENU_ITEMS] = {
        "1) Play",
        "2) About",
        "3) Exit"
};

const char * const CREDITS = "Created by Dylan Frendo";

int main(int argc, char *argv[]) {
    initscr();
    // Allows more control for the input.
    cbreak();
    // Do not display inserted keys to the screen.
    noecho();
    // Creates the border for the game.
    createOutsideBorder();
    // Show the Border.
    refresh();
    mainMenu();
    // Stop display.
    endwin();
    return 0;
}

void createOutsideBorder() {
    // Characters will be added to the main window.
    // Draw top and bottom borders.
    for(int x = 0; x < MAIN_WINDOW_COLUMN; x++) {
        mvaddch(0, x, MAIN_MENU_BORDER_CHARACTER);
        mvaddch(MAIN_WINDOW_ROW, x, MAIN_MENU_BORDER_CHARACTER);
    }
    // Draw left and right borders.
    for(int y = 0; y <= MAIN_WINDOW_ROW; y++) {
        mvaddch(y, 0, MAIN_MENU_BORDER_CHARACTER);
        mvaddch(y, MAIN_WINDOW_COLUMN, MAIN_MENU_BORDER_CHARACTER);
    }
}

void mainMenu() {
    int charSelected;
    // Display the main menu
    WINDOW * mainMenu = createMainMenuWindow();
    wrefresh(mainMenu);

    while (true) {
        // Let the user select a menu option
        charSelected = getch();

        switch (charSelected) {
            case '1':
                clearWindow(mainMenu);
                queueConnectionManager(mainMenu);
                break;
            case '2':
                delwin(mainMenu);
                break;
            case '3':
                delwin(mainMenu);
                break;
            default:
                continue;
        }
        if (charSelected == '3') {
            break;
        } else {
            // User went back to the main menu, re-draw the screen.
            mainMenu = createMainMenuWindow();
            wrefresh(mainMenu);
        }
    }
}

WINDOW * createMainMenuWindow() {
    int windowStartingX = MAIN_WINDOW_COLUMN / 4, windowStartingY = MAIN_WINDOW_ROW / 4, height = MAIN_WINDOW_ROW / 2, width = MAIN_WINDOW_COLUMN / 2;

    WINDOW * menuWindow;
    // Create new window where main menu will be placed.
    menuWindow = newwin(height, width, windowStartingY, windowStartingX);

    // Draw top and bottom borders and the divisor line.
    for(int x = 0; x < width; x++) {
        mvwaddch(menuWindow, 0, x, MAIN_MENU_BORDER_CHARACTER);
        mvwaddch(menuWindow, height - 1, x, MAIN_MENU_BORDER_CHARACTER);
        // Add the divisor line
        mvwaddch(menuWindow, height - 3, x, MAIN_MENU_DIVIDER);
    }
    // Draw left and right borders.
    for(int y = 0; y <= height; y++) {
        mvwaddch(menuWindow, y, 0, MAIN_MENU_BORDER_CHARACTER);
        mvwaddch(menuWindow, y, width - 1, MAIN_MENU_BORDER_CHARACTER);
    }
    // Print menu items.
    for (int i = 0; i < MAIN_MENU_ITEMS; i++) {
        mvwprintw(menuWindow, 1 + i, 2, MENU_ITEMS[i]);
    }
    // Add Credits
    mvwprintw(menuWindow, height - 2, 2, CREDITS);
    return menuWindow;
}
