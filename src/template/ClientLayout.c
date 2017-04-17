//
// Created by dylan on 15/04/2017.
//
#include "ClientLayout.h"
#include "../utility/General.h"
#include "WindowProperties.h"
#include "../server/Server.h"

void generateWindowForWaitingInQueue(Vector * connections, WINDOW * window) {

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
    wrefresh(window);
}