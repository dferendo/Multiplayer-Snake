//
// Created by dylan on 15/04/2017.
//

#ifndef SNAKES_SERIALIZE_H
#define SNAKES_SERIALIZE_H

#include "../server/Server.h"
#include "LinkedList.h"
#include "../server/Snake.h"

#define INTEGER_BYTES 4
#define POSITION_BYTES 8
#define FOOD_BYTES_SIZE 12

#define DELIMITERS_SIZE 2
#define SNAKE_DETAILS_DELIMITER "/S"
#define VECTOR_OF_FOOD_DELIMITER "/F"
#define CHANGE_DIRECTION_DELIMITER "/A"
#define WINNER_DELIMITER "/W"
#define LOSE_DELIMITER "/L"
#define RESTART_DELIMITER "/R"
#define UNIQUE_ID_DELIMITER "/U"

// Note that 1 character is 1 byte.

// General serialize.

unsigned char * serializeInt(unsigned char * buffer, int value);

unsigned char * deserializeInt(unsigned char *buffer, int * value);

unsigned char * serializeCharArray(unsigned char * buffer, char * value, int size);

// Serialize/Deserialize Snake information.

unsigned char * serializedLinkedList(unsigned char * buffer, LinkedListPosition * linkedListPosition);

unsigned char * serializedSnake(unsigned char * buffer, Snake * snake);

unsigned char * deserializedSnake(unsigned char *buffer, Snake *snake, int size);

// Serialize/Deserialize Position information.

unsigned char * serializedPosition(unsigned char * buffer, Position * position);

unsigned char * deserializedPosition(unsigned char * buffer, Position * position);

// Serialize/Deserialize Food information.

unsigned char * serializedVectorOfFoodsWithDelimiter(unsigned char * buffer, Vector * foods);

unsigned char * deserializedVectorOfFoods(unsigned char * buffer, Vector * foods, int size);

// Serialize/Deserialize Direction information.

unsigned char * serializedSnakeDirectionWithDelimiter(unsigned char * buffer, int direction);

unsigned char * serializedSnakesFromConnections(unsigned char *buffer, Vector *connections);

// Serialize User ID

unsigned char * serializedUserUniqueID(unsigned char * buffer, int uniqueID);

#endif //SNAKES_SERIALIZE_H
