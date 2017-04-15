//
// Created by dylan on 15/04/2017.
//
#include "General.h"
#include "../template/WindowProperties.h"
#include "../template/ClientLayout.h"

WINDOW * createWindowAtTheCenterOfTheScreen() {
    WINDOW * tempWindow;
    // StartingX and Y put in centre (Approx)
    int height = 4, width = MAXIMUM_INPUT_STRING + PLAY_GAME_MENU_LENGTH,
            startingX = (COLUMN / 2) - (width / 2), startingY = (ROW / 2) - height;
    // Create new window where main menu will be placed.
    tempWindow = newwin(height, width, startingY, startingX);
    return tempWindow;
}