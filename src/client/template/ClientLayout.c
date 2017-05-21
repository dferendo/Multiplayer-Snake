#include <strings.h>
#include "ClientLayout.h"
#include "../../settings/GameSettings.h"
#include "unistd.h"
#include "../SnakesGame.h"

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
    sleep(PROMPT_SCREEN_DELAY_SEC);
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

WINDOW * displayNewData(Vector *foods, Vector * snakes, int uniqueID) {
    WINDOW * window;
    Food * food;
    LinkedListPosition * snake;
    SnakeInfo * snakeInfo = NULL;
    Position * centrePosition = NULL;
    int totalRow, totalColumn, startingRow = 0, startingColumn = 0;

    getmaxyx(stdscr, totalRow, totalColumn);

    for (int i = 0; i < snakes->size; i++) {
        snakeInfo = ((SnakeInfo *) snakes->data[i]);

        if (snakeInfo->uniqueID == uniqueID) {
            centrePosition = snakeInfo->snake->positions->position;
            startingRow = centrePosition->y - (totalRow / 2);
            startingColumn = centrePosition->x - (totalColumn / 2);
            break;
        }
    }

    if (snakeInfo == NULL) {
        // Not found
        return NULL;
    }

    // Generate window.
    window = newwin(totalRow, totalColumn, 0, 0);

    // Check the snakes and food if they are found in the window screen

//    if (foods != NULL) {
//        for (int i = 0; i < foods->size; i++) {
//            food = ((Food *) foods->data[i]);
//            if (checkIfPositionIsFoundInScreen(centrePosition, food->position, totalRow, totalColumn)) {
//                mvwaddch(window, food->position->y, food->position->x, foodType(food));
//            }
//        }
//    }

    // Food can be generated before snakes
    if (snakes != NULL) {
        // Display Snakes for every connection if they are on screen
        for (int i = 0; i < snakes->size; i++) {
            snake = ((SnakeInfo *) snakes->data[i])->snake->positions;
            // Display snake.
            do {
                if (checkIfPositionIsFoundInScreen(centrePosition, snake->position, totalRow, totalColumn)) {
                    mvwprintw(window, snake->position->y - startingRow,
                              snake->position->x - startingColumn, SNAKE_CHARACTER);
                }
                snake = snake->next;
            } while (snake != NULL);
        }
    }

    // Create Border
    // Top border
    if (centrePosition->y - (totalRow / 2) < 0) {
        startingRow = centrePosition->y - (totalRow / 2);
        for (int i = 0; i < totalColumn; i++) {
            mvwaddch(window, abs(startingRow) - 1, i, '#');
        }
    }
    // Button border
    if (centrePosition->y + (totalRow / 2) > MAIN_WINDOW_ROW) {
        startingRow = (MAIN_WINDOW_ROW - centrePosition->y) + (totalRow / 2);
        for (int i = 0; i < totalColumn; i++) {
            mvwaddch(window, startingRow - 2, i, '#');
        }
    }
    // Left border
    if (centrePosition->x - (totalColumn / 2) < 0) {
        startingColumn = centrePosition->x - (totalColumn / 2);
        for (int i = 0; i < totalRow; i++) {
            mvwaddch(window, i, abs(startingColumn) - 1, '#');
        }
    }
    // Right border
    if (centrePosition->x + (totalColumn / 2) > MAIN_WINDOW_COLUMN) {
        startingColumn = (MAIN_WINDOW_COLUMN - centrePosition->x) + (totalColumn / 2);
        for (int i = 0; i < totalRow; i++) {
            mvwaddch(window, i, startingColumn - 2, '#');
        }
    }
    return window;
}

void showScreenInCentre(char *text) {
    WINDOW * tempWindow = createWindowAtTheCentreOfTheScreen(1);
    mvwprintw(tempWindow, 2, 3, text);
    wrefresh(tempWindow);
    sleep(PROMPT_SCREEN_DELAY_SEC);
    deleteWindow(tempWindow);
    delwin(tempWindow);
}

bool checkIfPositionIsFoundInScreen(Position * centre, Position * newPosition, int totalRows, int totalColumns) {
    int startingRow, startingColumn;
    startingRow = centre->y - (totalRows / 2);
    startingColumn = centre->x - (totalColumns / 2);

    if (!(newPosition->x > startingColumn && newPosition->x < (startingColumn + totalColumns))) {
        return false;
    }

    if (!(newPosition->y > startingRow && newPosition->y < (startingRow + totalRows))) {
        return false;
    }
    return true;
}
