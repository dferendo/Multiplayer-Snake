//
// Created by dylan on 17/04/2017.
//

#ifndef SNAKES_GAMESETTINGS_H
#define SNAKES_GAMESETTINGS_H

#define DEFAULT_START_DIRECTION D_UP
#define DEFAULT_START_DIRECTION_KEY 'w'
#define DEFAULT_START_SIZE 3

#define HOST_START_GAME_DELAY 2000000

#define MAXIMUM_FOOD_INTERVAL_SECS_US 3000000
#define MINIMUM_FOOD_INTERVAL_SECS_US 1000000
// Set 0 for no limit
#define MAXIMUM_AMOUNT_OF_FOOD_ON_SCREEN 20

#define FOOD_TO_WIN 15

#define GAME_UPDATE_RATE_US 200000

// All consoles will have the same size of the map
#define MAIN_WINDOW_ROW 24
#define MAIN_WINDOW_COLUMN 89

#define MAXIMUM_INPUT_STRING 20

#define DEAD_WIN_SCREEN_DELAY_SEC 3

#endif //SNAKES_GAMESETTINGS_H
