#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include "snakes-components/Game.h"

#include "snakes-components/Food.h"
#include "snakes-components/GameBoard.h"

int main() {
    srand((unsigned int) time(NULL));

    initscr();
    // Allows more control for the input.
    cbreak();
    // Do not display inserted keys to the screen.
    noecho();
    // Creates the border for the game.
    createBorder();
    // Show the Border.
    refresh();
    // getch() returns ERR if no input is present and does not block the thread.
    nodelay(stdscr, TRUE);

    WINDOW * snakesWindow = createSnakeRoamingWindow();
    Snake * snake = createSnake(snakesWindow);

    foodInit(snakesWindow);
    getch();
    return 0;
}