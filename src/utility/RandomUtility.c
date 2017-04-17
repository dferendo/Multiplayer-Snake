//
// Created by dylan on 12/04/2017.
//

#include "RandomUtility.h"
#include "../template/WindowProperties.h"

Position * createNewRandomPosition(Vector * positionsTaken) {
    int x, y;

    while (true) {
        // There is a border hence the -1.
        x = rand() % (ROW - 1);
        y = rand() % (COLUMN - 1);

        // Check if position is taken.
        for (int i = 0; i < positionsTaken->size; i++) {
            Position * temp = (Position *) positionsTaken->data[i];
            if (temp->x == x && temp->y == y) {
                continue;
            }
        }

        Position * position = (Position *) malloc(sizeof(Position));

        if (position == NULL) {
            perror("Failed to allocate memory to Position of Food.");
            return NULL;
        }
        position->x = x;
        position->y = y;
        // Add new position to vector.
        addItemToVector(positionsTaken, position);
        return position;
    }
}
