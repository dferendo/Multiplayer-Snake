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
                snake->direction = D_UP;
                break;
            case 'a':
                snake->direction = D_LEFT;
                break;
            case 'd':
                snake->direction = D_RIGHT;
                break;
            case 'x':
                snake->direction = D_DOWN;
            case ERR:
                break;
            default:
                continue;
        }
        usleep(300000);
        moveSnake(snakesWindow, snake);
    }
}
