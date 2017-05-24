#include <strings.h>
#include "ClientLayout.h"
#include "../../settings/GameSettings.h"
#include "unistd.h"
#include "../SnakesGame.h"
#include <stdlib.h>

#define SNAKE_COLOR_AVAILABLE 5

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
    setColors();
    // Set cursor to invisible
    curs_set(0);
    // Allows more control for the input
    cbreak();
    // Do not display inserted keys to the screen.
    noecho();
}

void setColors() {
    init_color(COLOR_BLACK, 150, 120, 120);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_color(COLOR_RED, 600, 200, 200);
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_YELLOW);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    init_pair(4, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(5, COLOR_WHITE, COLOR_CYAN);
    init_pair(6, COLOR_WHITE, COLOR_GREEN);
}

void printError(char *errorMessage) {
    WINDOW * window = createModalLayout(2);

    mvwprintw(window, 2, 3, errorMessage);
    mvwprintw(window, 3, 3, "Program exiting");
    wrefresh(window);
    sleep(PROMPT_SCREEN_DELAY_SEC);
    // Remove border
    wborder(window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(window);
    deleteWindow(window);
}

void deleteWindow(WINDOW *window) {
    if (window == NULL) {
        return;
    }
    wclear(window);
    wrefresh(window);
    delwin(window);
}

const chtype foodType(Food * type) {
    chtype foodTypes[] = {ACS_DIAMOND};
    return (const chtype) foodTypes[type->foodType];
}

WINDOW * createModalLayout(int height) {
    int totalRowVisitable, totalColumnVisitable, windowStartingX,
            windowStartingY, correctHeight = height + 4, width;
    getmaxyx(stdscr, totalRowVisitable, totalColumnVisitable);

    wborder(stdscr,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER,
            MAIN_MENU_BORDER_CHARACTER, MAIN_MENU_BORDER_CHARACTER);
    refresh();

    windowStartingX = totalColumnVisitable / 4, windowStartingY =  totalRowVisitable / 3,
    width = totalColumnVisitable / 2;

    WINDOW * menuWindow;
    // Create new window where main menu will be placed.
    menuWindow = newwin(correctHeight, width, windowStartingY, windowStartingX);
    wbkgd(menuWindow, COLOR_PAIR(7));
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
    int totalRowVisitable, totalColumnVisitable,
            startingRow = 0, startingColumn = 0,
            startColOrRow, endColOrRow, statisticDataLength = 22, snakeColor;
    bool isFirstPosition;
    char buffer[statisticDataLength];

    getmaxyx(stdscr, totalRowVisitable, totalColumnVisitable);

    if (snakes != NULL) {
        for (int i = 0; i < snakes->size; i++) {
            snakeInfo = ((SnakeInfo *) snakes->data[i]);

            if (snakeInfo->uniqueID == uniqueID) {
                centrePosition = snakeInfo->snake->positions->position;
                startingRow = centrePosition->y - (totalRowVisitable / 2);
                startingColumn = centrePosition->x - (totalColumnVisitable / 2);
                break;
            }
        }
    }

    if (centrePosition == NULL) {
        // Not found, error
        return NULL;
    }

    // Generate window.
    window = newwin(totalRowVisitable, totalColumnVisitable, 0, 0);

    wbkgd(window, COLOR_PAIR(7));

    // Check the snakes and food if they are found in the window screen
    if (foods != NULL) {
        for (int i = 0; i < foods->size; i++) {
            food = ((Food *) foods->data[i]);
            if (checkIfPositionIsFoundInScreen(centrePosition, food->position, totalRowVisitable, totalColumnVisitable)) {
                mvwaddch(window, food->position->y - startingRow,
                         food->position->x - startingColumn, foodType(food));
            }
        }
    }

    // Food can be generated before snakes
    if (snakes != NULL) {
        // Display Snakes for every connection if they are on screen
        for (int i = 0; i < snakes->size; i++) {
            snake = ((SnakeInfo *) snakes->data[i])->snake->positions;
            if (((SnakeInfo *) snakes->data[i])->uniqueID == uniqueID) {
                snakeColor = 1;
                wattron(window, COLOR_PAIR(snakeColor));
            } else {
                snakeColor = (((SnakeInfo *) snakes->data[i])->uniqueID % SNAKE_COLOR_AVAILABLE) + 2;
                wattron(window, COLOR_PAIR(snakeColor));
            }
            isFirstPosition = true;
            // Display snake.
            do {
                if (checkIfPositionIsFoundInScreen(centrePosition, snake->position, totalRowVisitable, totalColumnVisitable)) {
                    if (isFirstPosition) {
                        mvwaddch(window, snake->position->y - startingRow,
                                 snake->position->x - startingColumn, DIR((((SnakeInfo *) snakes->data[i]))->snake->direction));
                    } else {
                        mvwaddch(window, snake->position->y - startingRow,
                                 snake->position->x - startingColumn, SNAKE_CHARACTER);
                    }
                }
                isFirstPosition = false;
                snake = snake->next;
            } while (snake != NULL);
            wattroff(window, COLOR_PAIR(snakeColor));
        }
    }

    // Create Border
    // Top border
    if (centrePosition->y - (totalRowVisitable / 2) < 0) {
        startingRow = centrePosition->y - (totalRowVisitable / 2);
        // indicate the starting and ending of the border. The border can stay not at the
        // beginning (not 0) if the player is near the corner.
        startColOrRow = centrePosition->x - (totalColumnVisitable / 2) < 0 ? centrePosition->x - (totalColumnVisitable / 2) : 0;
        endColOrRow = centrePosition->x + (totalColumnVisitable / 2) > MAIN_WINDOW_COLUMN ?
                      (MAIN_WINDOW_COLUMN - centrePosition->x) + (totalColumnVisitable / 2) : totalColumnVisitable;
        for (int i = abs(startColOrRow); i < endColOrRow; i++) {
            mvwaddch(window, abs(startingRow) - 1, i, MAIN_MENU_BORDER_CHARACTER);
        }
    }
    // Button border
    if (centrePosition->y + (totalRowVisitable / 2) >= MAIN_WINDOW_ROW - 1) {
        startingRow = (MAIN_WINDOW_ROW - centrePosition->y) + (totalRowVisitable / 2);

        startColOrRow = centrePosition->x - (totalColumnVisitable / 2) < 0 ? centrePosition->x
                                                                             - (totalColumnVisitable / 2) : 0;
        endColOrRow = centrePosition->x + (totalColumnVisitable / 2) > MAIN_WINDOW_COLUMN ?
                      (MAIN_WINDOW_COLUMN - centrePosition->x) + (totalColumnVisitable / 2) : totalColumnVisitable;
        for (int i = abs(startColOrRow); i < endColOrRow; i++) {
            mvwaddch(window, startingRow - 1, i, MAIN_MENU_BORDER_CHARACTER);
        }
    }
    // Left border
    if (centrePosition->x - (totalColumnVisitable / 2) < 0) {
        startingColumn = centrePosition->x - (totalColumnVisitable / 2);

        startColOrRow = centrePosition->y - (totalRowVisitable / 2) < 0 ? centrePosition->y - (totalRowVisitable / 2) : 0;
        endColOrRow = centrePosition->y + (totalRowVisitable / 2) >= MAIN_WINDOW_ROW ?
                      (MAIN_WINDOW_ROW - centrePosition->y) + (totalRowVisitable / 2) : totalRowVisitable;
        for (int i = abs(startColOrRow) - 1; i < endColOrRow; i++) {
            mvwaddch(window, i, abs(startingColumn) - 1, MAIN_MENU_BORDER_CHARACTER);
        }
    }
    // Right border
    if (centrePosition->x + (totalColumnVisitable / 2) > MAIN_WINDOW_COLUMN) {
        startingColumn = (MAIN_WINDOW_COLUMN - centrePosition->x) + (totalColumnVisitable / 2);

        startColOrRow = centrePosition->y - (totalRowVisitable / 2) < 0 ? centrePosition->y - (totalRowVisitable / 2) : 0;
        endColOrRow = centrePosition->y + (totalRowVisitable / 2) >= MAIN_WINDOW_ROW ?
                      (MAIN_WINDOW_ROW - centrePosition->y) + (totalRowVisitable / 2) : totalRowVisitable;
        for (int i = abs(startColOrRow); i < endColOrRow; i++) {
            mvwaddch(window, i, startingColumn - 1, MAIN_MENU_BORDER_CHARACTER);
        }
    }
    sprintf(buffer, "(%d, %d) Score: %d", centrePosition->x, centrePosition->y, snakeInfo->snake->size);
    mvwprintw(window, 0, totalColumnVisitable - statisticDataLength, buffer);

    return window;
}

void showScreenInCentre(char *text) {
    WINDOW * tempWindow = createModalLayout(1);
    mvwprintw(tempWindow, 2, 3, text);
    wrefresh(tempWindow);
    sleep(PROMPT_SCREEN_DELAY_SEC);
    wborder(stdscr, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    refresh();
    deleteWindow(tempWindow);
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
