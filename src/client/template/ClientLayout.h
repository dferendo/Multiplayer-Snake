//
// Created by dylan on 14/04/2017.
//

#ifndef SNAKES_CLIENTLAYOUT_H
#define SNAKES_CLIENTLAYOUT_H

#include <ncurses.h>
#include "../../utility/Vector.h"
#include "../../server/Food.h"

// Put 0 for clean border
#define MAIN_MENU_BORDER_CHARACTER '+'
#define SNAKE_CHARACTER "#"

#define ERROR_NO_HOST "Error, no such host!"
#define ERROR_CONNECTION_FAILED "Error, connection failed!"

#define WINNER_TEXT "You Win!!"
#define DIED_TEXT "You Died"
#define RESTART_TEXT "Another snake won, restarting"

/**
 * Setting up of ncurses used. Exit the program if colours is not supported by terminal.
 */
void ncursesInit();

/**
 * Create an outside border on the standard screen.
 */
void createOutsideBorder();

/**
 * Print given text in the centre of the screen, used to inform the user
 * that the program will exits.
 *
 * @param errorMessage: The type of error message.
 */
void printError(char *errorMessage);

/**
 * Generate the window used by the snakes to roam the playing area.
 *
 * @return: The window where the snakes and foods will be placed.
 */
WINDOW * generatePlayingWindow();

/**
 * Clears and delete passed WINDOW.
 *
 * @param window: The window to be deleted.
 */
void deleteWindow(WINDOW *window);

/**
 * Creates a window at the centre with a border.
 *
 * @param height: The amount of rows the user need.
 * @return: Returns the window at the centre of the screen.
 */
WINDOW * createWindowAtTheCentreOfTheScreen(int height);

/**
 * Returns the food type of a Food in chtype format.
 *
 * @param type: The food.
 * @return: The type of the food.
 */
const chtype foodType(Food * type);

/**
 * Displays the data of foods and snakes, window requires still wrefresh.
 *
 * @param foods: The current foods.
 * @param snakes: The current positions of the snakes.
 * @return: Window with the data in the window.
 */
WINDOW * displayNewData(Vector * foods, Vector * snakes);

/**
 * Shows given text in the centre of the screen with an outside border.
 *
 * @param text: The text to be displayed.
 */
void showScreenInCentre(char *text);

#endif //SNAKES_CLIENTLAYOUT_H
