//
// Created by dylan on 11/04/2017.
//
#include <pthread.h>
#include <unistd.h>
#include "Food.h"
#include "Game.h"
#include "../settings/GameSettings.h"
#include "../utility/RandomUtility.h"
#include "API/SnakesAPI.h"

pthread_mutex_t lock;

void * generateFood(void * arg) {
    Vector * foods = ((FoodGeneratorParams *) arg)->foods;
    Vector * connections = ((FoodGeneratorParams *) arg)->connections;
    bool * keepAlive = ((FoodGeneratorParams *) arg)->killThread;
    Food * food;
    Position * position;
    int nextFoodGenerator;
    bool error;

    while (true) {
        if (!(*keepAlive)) {
            free(arg);
            pthread_exit(NULL);
        }
        pthread_mutex_lock(&lock);
        // If no connections, do not generate food
        if (connections->size != 0) {
            if (MAXIMUM_AMOUNT_OF_FOOD_ON_SCREEN == 0 ||
                foods->size <= MAXIMUM_AMOUNT_OF_FOOD_ON_SCREEN) {

                position = createFoodPosition(connections, foods);

                if (position == NULL) {
                    pthread_mutex_unlock(&lock);
                    continue;
                }

                food = (Food *) malloc(sizeof(Food));

                if (food == NULL) {
                    pthread_mutex_unlock(&lock);
                    perror("Failed to allocate memory to Food");
                    free(position);
                    continue;
                }

                food->position = position;
                food->foodType = F_NORMAL;

                if (addItemToVector(foods, food) < 0) {
                    pthread_mutex_unlock(&lock);
                    free(position);
                    free(food);
                    continue;
                }
                // Write to clients about the foods.
                do {
                    // Send data again, if a connection is lost re-send the data.
                    error = writeFoodDataToClients(connections, foods);;
                } while (!error);
            }
        }
        pthread_mutex_unlock(&lock);
        // Sleep thread
        nextFoodGenerator = rand() % (MAXIMUM_FOOD_INTERVAL_SECS_US + 1 -
                                      MINIMUM_FOOD_INTERVAL_SECS_US) + MINIMUM_FOOD_INTERVAL_SECS_US;
        usleep((__useconds_t) nextFoodGenerator);
    }

}