//
// Created by dylan on 12/04/2017.
//

#include "RandomUtility.h"
#include <stdlib.h>
#include "../snakes-components/CharactersDefinition.h"

Position * createNewRandomPosition(WINDOW *window) {
    int maxX, maxY, x, y, checkCharacter;

    while (true) {
        maxX = getmaxx(window);
        maxY = getmaxy(window);

        x = rand() % (maxX + 1);
        y = rand() % (maxY + 1);

        checkCharacter = mvwgetch(window, x, y);

        // If there is someone on the screen, the position is not good thus re-try
        if (checkCharacter == SNAKE_BODY || checkCharacter == BORDER_CHARACTER || checkCharacter == F_NORMAL) {
            continue;
        }
        Position * position = (Position *) malloc(sizeof(Position));

        if (position == NULL) {
            perror("Failed to allocate memory to Position of Food.");
            return NULL;
        }
        position->x = x;
        position->y = y;
        return position;
    }
}
