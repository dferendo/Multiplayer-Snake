#include <strings.h>
#include "ClientLayout.h"
#include "../../settings/GameSettings.h"
#include "unistd.h"

void ncursesInit() {
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
}

void createOutsideBorder() {
    // Create border
    wborder(stdscr,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER);
}

void printError(char *errorMessage) {
    WINDOW * window = createWindowAtTheCentreOfTheScreen(2);

    mvwprintw(window, 2, 3, errorMessage);
    mvwprintw(window, 3, 3, "Program exiting");
    wrefresh(window);
    sleep(PROMPT_SCREEN_DELAY);
    deleteWindow(window);
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

const chtype foodType(Food * type) {
    chtype foodTypes[] = {ACS_DIAMOND};
    return (const chtype) foodTypes[type->foodType];
}

WINDOW * createWindowAtTheCentreOfTheScreen(int height) {
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

WINDOW *displayNewData(Vector *foods, Vector * snakes) {
    WINDOW * window = generatePlayingWindow();
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

void showScreenInCentre(char *text) {
    WINDOW * tempWindow = createWindowAtTheCentreOfTheScreen(1);
    mvwprintw(tempWindow, 2, 3, text);
    wrefresh(tempWindow);
    sleep(PROMPT_SCREEN_DELAY);
    deleteWindow(tempWindow);
    delwin(tempWindow);
}
