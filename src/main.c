#include <ncurses.h>
#include <stdlib.h>
#include "snakes-components/Game.h"

#include "snakes-components/Food.h"
#include "Utility/Vector.h"

int main() {
//    nCursesInit();
    vectorFood * temp = initModifiedVectorFood();
    Position * position = (Position *) malloc(sizeof(Position));
    position->x = 5;
    position->y = 6;
    Food * food = (Food *) malloc(sizeof(Food));
    food->position = position;
    food->foodType = NORMAL;

    Position * position3 = (Position *) malloc(sizeof(Position));
    position3->x = 4;
    position3->y = 2;
    Food * food2 = (Food *) malloc(sizeof(Food));
    food2->position = position3;
    food2->foodType = NORMAL;

    addFoodToModifiedVector(temp, food);
    addFoodToModifiedVector(temp, food2);
    deleteItem(temp, food);

    deleteVector(temp);
    return 0;
}