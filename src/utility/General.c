//
// Created by dylan on 15/04/2017.
//
#include <string.h>
#include "General.h"
#include "../template/WindowProperties.h"
#include "../server/Server.h"

WINDOW * createWindowAtTheCenterOfTheScreen(int height, int width) {
    WINDOW * tempWindow;
    // StartingX and Y put in centre (Approx)
    int startingX = (COLUMN / 2) - (width / 2), startingY = (ROW / 2) - height;
    // Create new window where main menu will be placed.
    tempWindow = newwin(height, width, startingY, startingX);
    return tempWindow;
}

bool checkIfHost(Vector *connections, char *playerID) {

    for (int i = 0; i < connections->size; i++) {
        Connection * connection = (Connection *) connections->data[i];

        if (strcmp(connection->clientInfo->name, playerID) == 0 &&
                connection->clientInfo->isHost) {
            return true;
        }
    }
    return false;
}