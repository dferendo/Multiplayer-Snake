//
// Created by dylan on 11/04/2017.
//
#include <pthread.h>
#include <unistd.h>
#include <strings.h>
#include "Food.h"
#include "Game.h"
#include "../settings/GameSettings.h"
#include "../utility/RandomUtility.h"
#include "../utility/Serialize.h"
#include "ServerHandle.h"
#include "../utility/General.h"

void * generateFood(void * arg) {
    Vector * foods = ((FoodGeneratorParams *) arg)->foods;
    Vector * connections = ((FoodGeneratorParams *) arg)->connections;
    pthread_mutex_t lock = ((FoodGeneratorParams *) arg)->lock;
    Food * food;
    Position * position;
    int nextFoodGenerator;
    bool error;

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
            do {
                // Send data again, if a connection is lost re-send the data.
                error = writeFoodDataToClients(connections, foods);;
            } while (!error);

            pthread_mutex_unlock(&lock);
        }
        // Sleep thread
        nextFoodGenerator = rand() % (MAXIMUM_FOOD_INTERVAL_SECS_US + 1 -
                                      MINIMUM_FOOD_INTERVAL_SECS_US) + MINIMUM_FOOD_INTERVAL_SECS_US;
        usleep((__useconds_t) nextFoodGenerator);
    }
}

bool writeFoodDataToClients(Vector * connections, Vector * foods) {
    int response;
    Connection * connection;
    size_t size = DELIMITERS_SIZE + INTEGER_BYTES + (FOOD_BYTES_SIZE * foods->size);
    unsigned char buffer[size];
    bzero(buffer, size);

    serializedVectorOfFoodsWithDelimiter(buffer, foods);

    for (int i = 0; i < connections->size; i++) {
        connection = (Connection *) connections->data[i];

        response = (int) write(connection->sockFd, buffer, size);

        if (response < 0) {
            perror("Error writing to socket");
            // Error, close socket, delete connection and indicate the server to
            // resend the data.
            freeConnection(connection);
            deleteItemFromVector(connections, connection);
            return false;
        }
    }
    return true;
}