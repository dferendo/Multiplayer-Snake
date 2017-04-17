//
// Created by dylan on 15/04/2017.
//
#include "ClientLayout.h"
#include "../utility/General.h"
#include "WindowProperties.h"
#include "../server/Server.h"

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
    window = newwin(ROW - 2, COLUMN - 2, 1, 1);
    return window;
}

void clearWindow(WINDOW *window) {
    wclear(window);
    wrefresh(window);
}
