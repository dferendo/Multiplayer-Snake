//
// Created by dylan on 17/04/2017.
//

#ifndef SNAKES_SNAKESGAME_H
#define SNAKES_SNAKESGAME_H

#include "../utility/Vector.h"
#include "../server/Snake.h"

/**
 * Params passed to the thread that reads user input to change the direction of the
 * snake.
 * sockFd: Used to communicate with the server.
 * keepAlive: Terminate Thread.
 */
typedef struct ReadUserInputThreadParams {
    int sockFd;
    bool * keepAlive;
} ReadUserInputThreadParams;

/**
 * Holds the information of a snake.
 * snake: snake with its position and size.
 * uniqueID: The ID of the snake.
 */
typedef struct SnakeInfo {
    Snake * snake;
    int uniqueID;
} SnakeInfo;

/**
 * Initialise variables/thread required to play the game. Creates a thread to read the input
 * from the user and leaves the main thread to read the data from the server. When the game
 * is over, it calls the appropriate methods to close all data used.
 *
 * @param sockFd: The socket file descriptor used to communicate with the server.
 * @return -1 if the game needs to stop, 1 to restart the game.
 */
int gameManager(int sockFd);

/**
 * Handles the next delimiter sends from the server. Delimiters used can be seen in
 * {@link Serialize.h}. If the received data matched no delimiter, the game stops.
 *
 * @param sockFd: The socket file descriptor used to communicate with the server.
 * @return: -1 indicates a connection error.
 *          1 indicates this client is a winner.
 *          2 indicates this client is a loser.
 *          3 indicates another player won this restart the game.
 */
int handleGameDataFromServer(int sockFd);

/**
 * Handles the resize of a signal.
 *
 * @param sig
 */
void handleResize(int sig);

/**
 * Handle any food update of the game and update the window screen. Will clean the
 * previous vector holding the previous foods of the game.
 *
 * @param sockFd: The socket file descriptor used to communicate with the server.
 * @return: False if there was a connection error, true otherwise.
 */
bool foodHandler(int sockFd);

/**
 * Handle snakes update of the game and update the window screen. Will clean
 * all the previous data of the snakes.
 *
 * @param sockFd: The socket file descriptor used to communicate with the server.
 * @return: False if there was a connection error, true otherwise.
 */
bool snakeHandler(int sockFd);

/**
 * Thread used to read the direction from user and send the direction to the server.
 * If connection failed, the thread will be killed.
 *
 * @param args: {@link ReadUserInputThreadParams} is passed.
 * @return: Nothing
 */
void * readDirectionFromUser(void *args);

/**
 * Free all the game data used throughout the game. It also closes the thread used to read
 * the characters. This function is called whenever the games restarts or connection is lost.
 *
 * @param snakes: Snake data.
 * @param foods: Food data.
 * @param keepAlive: Boolean to kill the thread.
 * @param characterReaderId: Thread to be killed.
 */
void removeGameData(Vector *snakes, Vector *foods, bool *keepAlive, pthread_t characterReaderId);

/**
 * Display the user the type of game ending he/she received. Will show the appropriate screen 
 * regarding whether the game ending with a win, lost, restart or connection lost depending on 
 * the passed parameter.
 * 
 * @param status: The status type indicating the end game.
 * @return: 1 indicating the game needs to be restarted, -1 exit program.
 */
int closeGame(int status);

#endif //SNAKES_SNAKESGAME_H
