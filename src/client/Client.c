
#include "Client.h"
#include "../template/ClientLayout.h"
#include "QueueMenu.h"

const char * const MENU_ITEMS[MAIN_MENU_ITEMS] = {
        "1) Play",
        "2) About",
        "3) Exit"
};

const char * const CREDITS = "Created by Dylan Frendo";

int main(int argc, char *argv[]) {
    initscr();
    // Check if terminal has colours
    if (has_colors() == FALSE) {
        endwin();
        printf("Terminal does not support colors!!\n");
        exit(1);
    }
    // Start the colours
    start_color();
    // Set cursor to invisible
    curs_set(0);
    // Allows more control for the input
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
                aboutMenu();
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