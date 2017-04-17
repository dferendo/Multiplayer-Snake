//
// Created by dylan on 15/04/2017.
//

#ifndef SNAKES_SERIALIZE_H
#define SNAKES_SERIALIZE_H

#include "../server/Server.h"

#define INTEGER_BYTES 4
#define CLIENTINFO_BYTES 22
#define CONNECTION_BYTES 26

#define DELIMITERS_SIZE 2
#define VECTOR_OF_CONNECTIONS_DELIMITER "/C"

unsigned char * serializeInt(unsigned char * buffer, int value);

unsigned char * serializeShort(unsigned char * buffer, short value);

unsigned char * serializeCharArray(unsigned char * buffer, char * value, int size);

unsigned char * serializeClientInfo(unsigned char * buffer, ClientInfo * clientInfo);

unsigned char * serializeConnection(unsigned char * buffer, Connection * connection);

unsigned char * serializeVectorOfConnections(unsigned char * buffer, Vector * connections);

unsigned char * serializedVectorOfConnectionsDelimiter(unsigned char * buffer, Vector *connections);

unsigned char * deserializeInt(unsigned char *buffer, int * value);

unsigned char * deserializeShort(unsigned char * buffer, short * value);

unsigned char * deserializeCharArray(unsigned char * buffer, char * value, int size);

unsigned char * deserializeClient(unsigned char * buffer, ClientInfo * clientInfo);

unsigned char * deserializeConnection(unsigned char * buffer, Connection * connection);

unsigned char * deserializeVectorOfConnections(unsigned char * buffer, Vector * connections, int size);

#endif //SNAKES_SERIALIZE_H
