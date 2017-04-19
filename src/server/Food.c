//
// Created by dylan on 11/04/2017.
//
#include <pthread.h>
#include <unistd.h>
#include <strings.h>
#include "Food.h"
#include "Server.h"
#include "../template/GameSettings.h"
#include "../utility/RandomUtility.h"
#include "../utility/Serialize.h"

Vector * foods;
Vector * connections;
pthread_mutex_t lock;

void * generateFood(void * arg) {
    Food * food;
    Position * position;
    int nextFoodGenerator;

    while (true) {

        if (MAXIMUM_AMOUNT_OF_FOOD_ON_SCREEN == 0 ||
                foods->size <= MAXIMUM_AMOUNT_OF_FOOD_ON_SCREEN) {
            pthread_mutex_lock(&lock);

            position = createFoodPosition(connections, foods);

            food = (Food *) malloc(sizeof(Food));

            if (food == NULL) {
                perror("Failed to allocate memory to Food");
                free(position);
                continue;
            }

            food->position = position;
            food->foodType = F_NORMAL;

            addItemToVector(foods, food);

            // Write to clients about the foods.
            writeFoodDataToClients(connections, foods);

            pthread_mutex_unlock(&lock);
        }
        // Sleep thread
        nextFoodGenerator = rand() % (MAXIMUM_FOOD_INTERVAL_SECS_US + 1 -
                MINIMUM_FOOD_INTERVAL_SECS_US) + MINIMUM_FOOD_INTERVAL_SECS_US;
        usleep((__useconds_t) nextFoodGenerator);
    }
}

void writeFoodDataToClients(Vector * connections, Vector * foods) {
    int response;
    size_t size = DELIMITERS_SIZE + INTEGER_BYTES + (FOOD_BYTES_SIZE * foods->size);
    unsigned char buffer[size];
    bzero(buffer, size);

    serializedVectorOfFoodsWithDelimiter(buffer, foods);

    for (int i = 0; i < connections->size; i++) {
        Connection * temp = (Connection *) connections->data[i];

        response = (int) write(temp->sockFd, buffer, size);

        if (response < 0) {
            perror("Error writing to socket");
            close(temp->sockFd);
        }
    }
}
