//
// Created by dylan on 15/04/2017.
//

#ifndef SNAKES_SERIALIZE_H
#define SNAKES_SERIALIZE_H

#include "../server/Server.h"

#define INTEGER_BYTES 4
#define CLIENTINFO_BYTES_NO_SNAKE 22
#define CONNECTION_BYTES_NO_SNAKE 26
#define POSITION_BYTES 8
#define SNAKE_BYTES_NO_POSITIONS 4
#define FOOD_BYTES_SIZE 12

#define DELIMITERS_SIZE 2
#define VECTOR_OF_CONNECTIONS_DELIMITER "/C"
#define SNAKE_DETAILS_DELIMETER "/S"
#define HOST_STARTS_GAME_DELIMETER "/M"
#define VECTOR_OF_FOOD_DELIMITER "/F"

unsigned char * serializeInt(unsigned char * buffer, int value);

unsigned char * serializeShort(unsigned char * buffer, short value);

unsigned char * serializeCharArray(unsigned char * buffer, char * value, int size);

unsigned char * serializeClientInfo(unsigned char * buffer, ClientInfo * clientInfo);

unsigned char * serializeConnection(unsigned char * buffer, Connection * connection);

unsigned char * serializeVectorOfConnections(unsigned char * buffer, Vector * connections);

unsigned char * serializedVectorOfConnectionsDelimiter(unsigned char * buffer, Vector *connections);

unsigned char * serializedPosition(unsigned char * buffer, Position * position);

unsigned char * serializedLinkedList(unsigned char * buffer, LinkedListPosition * linkedListPosition);

unsigned char * serializedSnake(unsigned char * buffer, Snake * snake);

unsigned char * deserializeInt(unsigned char *buffer, int * value);

unsigned char * deserializeShort(unsigned char * buffer, short * value);

unsigned char * deserializeCharArray(unsigned char * buffer, char * value, int size);

unsigned char * deserializeClient(unsigned char * buffer, ClientInfo * clientInfo);

unsigned char * deserializeConnection(unsigned char * buffer, Connection * connection);

unsigned char * deserializeVectorOfConnections(unsigned char * buffer, Vector * connections, int size);

/**
 * Serialize Food information.
 */

unsigned char * serializedVectorOfFoods(unsigned char * buffer, Vector * foods);

#endif //SNAKES_SERIALIZE_H
