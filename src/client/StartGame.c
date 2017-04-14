//
// Created by dylan on 14/04/2017.
//
#include "StartGame.h"
#include "../template/WindowProperties.h"
#include "../template/ClientLayout.h"
#include <ncurses.h>

const char * const SERVER_REQUIRED[PLAY_GAME_MENU_ITEMS] = {
        "Player Name: ",
        "Server Name: ",
        "Port Number: "
};

void startGameInit() {
    char playerName[MAXIMUM_INPUT_STRING], serverName[MAXIMUM_INPUT_STRING],
            port[MAXIMUM_INPUT_STRING];
    getInput(playerName, serverName, port);
}

void getInput(char *name, char *serverName, char *port) {
    // Display Input name Window
    WINDOW * detailsInput;
    // StartingX and Y put in centre (Approx)
    int height = 3, width = MAXIMUM_INPUT_STRING + PLAY_GAME_MENU_LENGTH,
            startingX = (COLUMN / 2) - (width / 2), startingY = (ROW / 2) - height;
    // Create new window where main menu will be placed.
    detailsInput = newwin(height, width, startingY, startingX);

    for (int i = 0; i < PLAY_GAME_MENU_ITEMS; i++) {
        mvwprintw(detailsInput, i, 0, SERVER_REQUIRED[i]);
    }

    wrefresh(detailsInput);
    // Enable user to write on screen
    echo();
    // Get name
    wmove(detailsInput, 0, PLAY_GAME_MENU_LENGTH);
    wgetstr(detailsInput, name);
    // Get Server name
    wmove(detailsInput, 1, PLAY_GAME_MENU_LENGTH);
    wgetstr(detailsInput, serverName);
    // Get port number
    wmove(detailsInput, 2, PLAY_GAME_MENU_LENGTH);
    wgetstr(detailsInput, name);
}
