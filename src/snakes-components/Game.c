//
// Created by dylan on 11/04/2017.
//
#include <ncurses.h>
#include "Game.h"
#include "Snake.h"
#include "GameBoard.h"
#include "unistd.h"

WINDOW * snakesWindow;

void nCursesInit() {
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

    snakesWindow = createSnakeRoamingWindow();
    Snake * snake = createSnake(snakesWindow);
    waitInput(snake);
    endwin();
}

void waitInput(Snake * snake) {
    int nextChar;

    while (true) {
        nextChar = getch();
        switch (nextChar) {
            case 'w':
                // If snake allowed to go opposite side, it would mean he is dead.
                if (snake->direction != D_DOWN) {
                    snake->direction = D_UP;
                }
                break;
            case 'a':
                if (snake->direction != D_RIGHT) {
                    snake->direction = D_LEFT;
                }
                break;
            case 'd':
                if (snake->direction != D_LEFT) {
                    snake->direction = D_RIGHT;
                }
                break;
            case 'x':
                if (snake->direction != D_UP) {
                    snake->direction = D_DOWN;
                }
            case ERR:
                break;
            default:
                continue;
        }
        usleep(300000);
        moveSnake(snakesWindow, snake);
    }
}
