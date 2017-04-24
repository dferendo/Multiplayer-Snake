//
// Created by dylan on 18/04/2017.
//

#ifndef SNAKES_GAME_H
#define SNAKES_GAME_H

#include "../utility/Vector.h"
#include "../server/Server.h"
#include "Snake.h"

/**
 * Change directions params passed to pthread.
 * connections: All current connections.
 * killThread: Used to kill the thread.
 */
typedef struct ChangeDirectionParams {
    Vector * connections;
    bool * killThread;
} ChangeDirectionParams;

/**
 * Food Generator params passed to pthread.
 * foods: All current foods.
 * connections: All current connections.
 * killThread: Used to kill the thread.
 */
typedef struct FoodGeneratorParams {
    Vector * foods;
    Vector * connections;
    bool * killThread;
} FoodGeneratorParams;

/**
 * The status of the snake.
 */
typedef enum {
    WINNER = 0,      // Snake wins, restarts game
    DIED = 1,        // Snake Died, disconnect client
    NORMAL = 2,      // Snake moves, moves snake
    RESTART = 3      // Another snake won, restart game
} SnakeStatus;

/**
 * Creates the food generator and change direction reader pthreads. It initialise
 * the params needed for the thread and starts the game loop. When the game loop
 * ends, clean up of the data/threads used is cleared.
 *
 * @return: Nothing.
 */
void * gameManagement(void *);

/**
 * Loops every game refresh rate specified in {@link GameSettings.h}. It calls the methods
 * used to move snakes and send the new data to all the clients.
 *
 * @param connections: All connections to clients.
 * @param foods: All current foods.
 */
void gameLoop(Vector *connections, Vector *foods);

/**
 * Clear the passed vectors.
 *
 * @param connections: All current Connections.
 * @param foods: All current Foods.
 *
 * @warning: Only snakes will be cleared in the Connections Vector.
 */
void clearDataUsedForGame(Vector *connections, Vector *foods);

/**
 * Moves all the snakes found in the Vector connections. Determine the next status
 * of the moved snakes and if there is a status that is not NORMAL, will send that
 * status to the client/s.
 *
 * @param connections: All current connections.
 * @param foods: All foods.
 * @return: True if there is a winner/s, false otherwise.
 */
bool moveSnakes(Vector *connections, Vector *foods);

/**
 * Terminate the threads given and wait for them to finish. Also calls the appropiate
 * method to clean the data used.
 *
 * @param keepChangeDirectionThread: Used to kill the changed direction thread.
 * @param keepFoodGeneratorThread: Used to kill the food generator thread.
 * @param changeDirectionThread: Thread Id.
 * @param foodThread: Thread Id.
 */
void gameCleanUp(bool * keepChangeDirectionThread, bool * keepFoodGeneratorThread,
                 pthread_t changeDirectionThread, pthread_t foodThread);
#endif //SNAKES_GAME_H
