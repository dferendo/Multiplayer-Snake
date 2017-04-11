//
// Created by dylan on 11/04/2017.
//
#include "GameBoard.h"
#include "Snake.h"

void createBorder() {
    // Draw top and bottom borders.
    for(int x = 0; x < COLUMN; x++) {
        mvaddch(0, x, BORDER_CHARACTER);
        mvaddch(ROW, x, BORDER_CHARACTER);
    }
    // Draw left and right borders.
    for(int y = 0; y <= ROW; y++) {
        mvaddch(y, 0, BORDER_CHARACTER);
        mvaddch(y, COLUMN, BORDER_CHARACTER);
    }
}

WINDOW * createSnakeRoamingWindow() {
    WINDOW * tempWindow;
    // Create a new window where snakes can play. The border will not be touch since
    // it will not be moved.
    tempWindow = newwin(ROW - 1, COLUMN - 1, 1, 1);
    return tempWindow;
}

void clearWindow(WINDOW *window) {
    wclear(window);
}

WINDOW *resetWindow(WINDOW *window) {
    clearWindow(window);
    return createSnakeRoamingWindow();
}
