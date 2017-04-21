#include <strings.h>
#include "ClientLayout.h"
#include "../../settings/GameSettings.h"
#include "../../server/Server.h"
#include "unistd.h"

const char * const MENU_ITEMS[MAIN_MENU_ITEMS] = {
        "1) Play",
        "2) About",
        "3) Exit"
};

const char * const SERVER_REQUIRED[PLAY_GAME_MENU_REQUIRED] = {
        "Server Name: ",
        "Port Number: "
};

const char * const CREDITS = "Created by Dylan Frendo";

void createOutsideBorder() {
    // Create border
    wborder(stdscr,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER);
}

WINDOW * createMainMenuWindow() {
    WINDOW * menuWindow = createWindowAtTheCenterOfTheScreen((MAIN_WINDOW_ROW / 2) - 4);

    // Add Divisor line.
    for(int x = 1; x < (MAIN_WINDOW_COLUMN / 2) - 1; x++) {
        mvwaddch(menuWindow, (MAIN_WINDOW_ROW / 2) - 3, x, MAIN_MENU_DIVIDER);
    }
    // Print menu items.
    for (int i = 0; i < MAIN_MENU_ITEMS; i++) {
        mvwprintw(menuWindow, 1 + i, 2, MENU_ITEMS[i]);
    }
    // Add Credits
    mvwprintw(menuWindow, (MAIN_WINDOW_ROW / 2) - 2, 2, CREDITS);
    return menuWindow;
}

void aboutMenu() {
    WINDOW * menuWindow = createWindowAtTheCenterOfTheScreen((MAIN_WINDOW_ROW / 2) - 4);

    mvwprintw(menuWindow, 1, 2, "A simple multiple snakes made using ncu-");
    mvwprintw(menuWindow, 2, 2, "rses library. Controls: W A D X for the ");
    mvwprintw(menuWindow, 3, 2, "respective direction of the snake. To p-");
    mvwprintw(menuWindow, 4, 2, "lay a SERVER is required where the host ");
    mvwprintw(menuWindow, 5, 2, "can start a game.");
    mvwprintw(menuWindow, (MAIN_WINDOW_ROW / 2) - 2, 2, "Press any key to continue");

    wrefresh(menuWindow);
    getch();
    // Delete
    delwin(menuWindow);
}

void getInput(char * serverName, char * port) {
    WINDOW * menuWindow = createWindowAtTheCenterOfTheScreen(PLAY_GAME_MENU_REQUIRED);

    // Clear char
    bzero(serverName, MAXIMUM_INPUT_STRING);
    bzero(port, MAXIMUM_INPUT_STRING);
    // Display Input name Window
    for (int i = 0; i < PLAY_GAME_MENU_REQUIRED; i++) {
        mvwprintw(menuWindow, i + 2, 3, SERVER_REQUIRED[i]);
    }

    wrefresh(menuWindow);
    // Enable user to write on screen
    echo();
    // Show cursor
    curs_set(1);
    // Get Server name
    wmove(menuWindow, 2, PLAY_GAME_MENU_LENGTH + 3);
    wgetnstr(menuWindow, serverName, MAXIMUM_INPUT_STRING);
    // Get port number
    wmove(menuWindow, 3, PLAY_GAME_MENU_LENGTH + 3);
    wgetnstr(menuWindow, port, MAXIMUM_INPUT_STRING);
    // Disable cursor
    curs_set(0);
    // Disable user write on screen
    noecho();
    deleteWindow(menuWindow);
}

bool printErrorAndOfferRetry(char *errorMessage) {
    WINDOW * window = createWindowAtTheCenterOfTheScreen(2);

    mvwprintw(window, 2, 3, errorMessage);
    mvwprintw(window, 3, 3, "Retry? (Y/n)");
    wrefresh(window);
    int retry = getch();
    if (retry == 'Y' || retry == 'y') {
        deleteWindow(window);
        return true;
    }
    deleteWindow(window);
    return false;
}

WINDOW *generatePlayingWindow() {
    WINDOW * window;
    // Creating playing window without borders.
    window = newwin(MAIN_WINDOW_ROW - 1, MAIN_WINDOW_COLUMN - 1, 1, 1);
    return window;
}

void deleteWindow(WINDOW *window) {
    wclear(window);
    wrefresh(window);
    delwin(window);
}

void showWinnerScreen() {
    WINDOW * tempWindow = createWindowAtTheCenterOfTheScreen(1);
    mvwprintw(tempWindow, 0, 0, "You Win!!");
    wrefresh(tempWindow);
    sleep(DEAD_WIN_SCREEN_DELAY_SEC);
    deleteWindow(tempWindow);
    delwin(tempWindow);
}

void showDeadScreen() {
    WINDOW * tempWindow = createWindowAtTheCenterOfTheScreen(1);
    mvwprintw(tempWindow, 0, 0, "You Died");
    wrefresh(tempWindow);
    sleep(DEAD_WIN_SCREEN_DELAY_SEC);
    deleteWindow(tempWindow);
    delwin(tempWindow);
}

const chtype foodType(Food * type) {
    chtype foodTypes[] = {ACS_DIAMOND};
    return (const chtype) foodTypes[type->foodType];
}

WINDOW * createWindowAtTheCenterOfTheScreen(int height) {
    int windowStartingX = MAIN_WINDOW_COLUMN / 4, windowStartingY = MAIN_WINDOW_ROW / 4,
            correctHeight = height + 4, width = MAIN_WINDOW_COLUMN / 2;

    WINDOW * menuWindow;
    // Create new window where main menu will be placed.
    menuWindow = newwin(correctHeight, width, windowStartingY, windowStartingX);
    // Draw border
    wborder(menuWindow,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER);
    return menuWindow;
}

void serverErrorScreen() {
    WINDOW * tempWindow = createWindowAtTheCenterOfTheScreen(1);
    mvwprintw(tempWindow, 2, 3, ERROR_CONNECTION_FAILED);
    wrefresh(tempWindow);
    sleep(DEAD_WIN_SCREEN_DELAY_SEC);
    deleteWindow(tempWindow);
    delwin(tempWindow);
}

WINDOW *displayNewData(Vector *foods, Vector * snakes) {
    WINDOW *window = generatePlayingWindow();
    Food * food;
    LinkedListPosition * snake;

    // There could be some connections but no food yet.
    if (foods != NULL) {
        // Display Foods.
        for (int i = 0; i < foods->size; i++) {
            food = (Food *) foods->data[i];
            mvwaddch(window, food->position->y, food->position->x,
                     foodType(food));
        }
    }

    // Food can be generated before snakes
    if (snakes != NULL) {
        // Display Snakes for every connection
        for (int i = 0; i < snakes->size; i++) {
            snake = ((Snake *) snakes->data[i])->positions;
            // Display snake.
            do {
                mvwprintw(window, snake->position->y, snake->position->x, SNAKE_CHARACTER);
                snake = snake->next;
            } while (snake != NULL);
        }
    }
    return window;
}