//
// Created by dylan on 12/04/2017.
//

#include "RandomUtility.h"
#include "../settings/GameSettings.h"
#include "../server/Food.h"
#include "../server/Server.h"
#include "../server/Snake.h"
#include "../server/ServerHandle.h"

Position * createInitialSnakeRandomPosition(Vector * connections, Vector * foods) {
    int x, y;
    bool exists;
    Connection * connection;
    Food * food;
    Position * position;

    while (true) {
        // Do not create a snake exactly near the border
        x = rand() % (MAIN_WINDOW_COLUMN - 1 - DEFAULT_START_SIZE) + DEFAULT_START_SIZE;
        y = rand() % (MAIN_WINDOW_ROW - 1 - DEFAULT_START_SIZE) + DEFAULT_START_SIZE;
        exists = false;

        // Check if position is taken by other snakes.
        for (int i = 0; i < connections->size; i++) {
            connection = (Connection *) connections->data[i];
            // Check if the next DEFAULT START SIZE are taken
            for (int j = 1; j < DEFAULT_START_SIZE; j++) {
                positionExistsLinkedList(connection->snake->positions, x + j, y, &exists);
                // Position is already taken by another snake.
                if (exists) {
                    break;
                }
            }
            if (exists) {
                break;
            }
        }
        // Position is already taken by another snake thus re-generate number
        if (exists) {
            continue;
        }

        position = (Position *) malloc(sizeof(Position));

        if (position == NULL) {
            perror("Failed to allocate memory to Position of Food.");
            return NULL;
        }
        position->x = x;
        position->y = y;

        // Check if position is taken by a food.
        for (int i = 0; i < foods->size; i++) {
            food = (Food *) foods->data[i];
            if (checkIfPositionsAreEqual(food->position, position)) {
                exists = true;
                break;
            }
        }
        // Position is taken by a food, re-generate
        if (exists) {
            free(position);
            continue;
        }

        return position;
    }
}

Position * createFoodPosition(Vector *positionsOfSnakes, Vector * foodLocations) {
    int x, y;
    bool positionExists;
    Position * position;

    while (true) {
        // There is a border hence the -1.
        x = rand() % (MAIN_WINDOW_COLUMN - 1);
        y = rand() % (MAIN_WINDOW_ROW - 1);

        // Check if position is taken.
        for (int i = 0; i < foodLocations->size; i++) {
            Food * temp = (Food *) foodLocations->data[i];
            if (temp->position->x == x && temp->position->y == y) {
                continue;
            }
        }

        // Check if position is taken by a snake
        for (int i = 0; i < positionsOfSnakes->size; i++) {
            positionExists = false;
            Snake * snake = ((Connection *) positionsOfSnakes->data[i])->snake;
            if (snake != NULL) {
                positionExistsLinkedList(snake->positions, x, y, &positionExists);
                if (positionExists) {
                    continue;
                }
            }
        }

        // Else position is good
        position = (Position *) malloc(sizeof(Position));

        if (position == NULL) {
            perror("Failed to allocate memory to Position of Food.");
            return NULL;
        }
        position->x = x;
        position->y = y;
        // Add new position to vector.
        return position;
    }
}
