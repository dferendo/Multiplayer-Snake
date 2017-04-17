//
// Created by dylan on 15/04/2017.
//
#include "General.h"
#include "../template/WindowProperties.h"
#include "../template/ClientLayout.h"

WINDOW * createWindowAtTheCenterOfTheScreen(int height, int width) {
    WINDOW * tempWindow;
    // StartingX and Y put in centre (Approx)
    int startingX = (COLUMN / 2) - (width / 2), startingY = (ROW / 2) - height;
    // Create new window where main menu will be placed.
    tempWindow = newwin(height, width, startingY, startingX);
    return tempWindow;
}

void clearWindow(WINDOW *window) {
    wclear(window);
    wrefresh(window);
}