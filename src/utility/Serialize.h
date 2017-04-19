//
// Created by dylan on 15/04/2017.
//

#ifndef SNAKES_SERIALIZE_H
#define SNAKES_SERIALIZE_H

#include "../server/Server.h"

#define INTEGER_BYTES 4
#define CONNECTION_BYTES_NO_SNAKE 26
#define POSITION_BYTES 8
#define FOOD_BYTES_SIZE 12

#define DELIMITERS_SIZE 2
#define VECTOR_OF_CONNECTIONS_DELIMITER "/C"
#define SNAKE_DETAILS_DELIMITER "/S"
#define HOST_STARTS_GAME_DELIMITER "/M"
#define VECTOR_OF_FOOD_DELIMITER "/F"
#define CHANGE_DIRECTION_DELIMITER "/W"

// General serialize.

unsigned char * serializeInt(unsigned char * buffer, int value);

unsigned char * deserializeInt(unsigned char *buffer, int * value);

unsigned char * serializeShort(unsigned char * buffer, short value);

unsigned char * deserializeShort(unsigned char * buffer, short * value);

unsigned char * serializeCharArray(unsigned char * buffer, char * value, int size);

unsigned char * deserializeCharArray(unsigned char * buffer, char * value, int size);

// Serialize/Deserialize Client information.

unsigned char * serializeClientInfo(unsigned char * buffer, ClientInfo * clientInfo);

unsigned char * deserializeClientInfo(unsigned char *buffer, ClientInfo *clientInfo);

// Serialize/Deserialize Connection information.

unsigned char * serializeConnection(unsigned char * buffer, Connection * connection);

unsigned char * deserializeConnection(unsigned char * buffer, Connection * connection);

unsigned char * serializeVectorOfConnections(unsigned char * buffer, Vector * connections);

unsigned char * deserializeVectorOfConnections(unsigned char * buffer, Vector * connections, int size);

unsigned char * serializedVectorOfConnectionsDelimiter(unsigned char * buffer, Vector *connections);

// Serialize/Deserialize Snake information.

unsigned char * serializedLinkedList(unsigned char * buffer, LinkedListPosition * linkedListPosition);

unsigned char * serializedSnake(unsigned char * buffer, Snake * snake);

unsigned char * serializedSnakeFromConnections(unsigned char * buffer, Vector * connections);

unsigned char * deserializedSnake(unsigned char *buffer, Snake *snake, int size);

unsigned char * deserializedNameAndSizeOfSnake(unsigned char * buffer, char * name, int * size);

// Serialize/Deserialize Position information.

unsigned char * serializedPosition(unsigned char * buffer, Position * position);

unsigned char * deserializedPosition(unsigned char * buffer, Position * position);

// Serialize/Deserialize Food information.

unsigned char * serializedVectorOfFoodsWithDelimiter(unsigned char * buffer, Vector * foods);

unsigned char * deserializedVectorOfFoods(unsigned char * buffer, Vector * foods, int size);

#endif //SNAKES_SERIALIZE_H
