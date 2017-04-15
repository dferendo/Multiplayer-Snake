//
// Created by dylan on 15/04/2017.
//
#include "ClientLayout.h"
#include "../utility/General.h"
#include "WindowProperties.h"
#include "../server/Server.h"

WINDOW * generateWindowForWaitingInQueue(Vector * connections) {
    // Around 10 characters are needed for the number with the name and if the host
    WINDOW * clientViewWindow = createWindowAtTheCenterOfTheScreen();
    mvwprintw(clientViewWindow, 0, 0, "Players waiting: ");

    for (int i = 0; i < connections->size; i++) {
        char str[MAXIMUM_INPUT_STRING + 10];
        Connection * nextConnection = (Connection *) connections->data[i];
        if (nextConnection->clientInfo->isHost) {
            sprintf(str, "%d) %s (H)", i + 1, nextConnection->clientInfo->name);
            mvwprintw(clientViewWindow, i + 1, 0, str);
        } else {
            sprintf(str, "%d) %s", i + 1, nextConnection->clientInfo->name);
            mvwprintw(clientViewWindow, i + 1, 0, str);
        }
    }
    return clientViewWindow;
}
