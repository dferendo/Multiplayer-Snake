//
// Created by dylan on 15/04/2017.
//

#ifndef SNAKES_SERIALIZE_H
#define SNAKES_SERIALIZE_H

#include "../server/Server.h"

#define CLIENTINFO_BYTES 22
#define CONNECTION BYTES 26

unsigned char * serializeInt(unsigned char * buffer, int value);

unsigned char * serializeShort(unsigned char * buffer, short value);

unsigned char * serializeChar(unsigned char *buffer, char value);

unsigned char * serializeCharArray(unsigned char * buffer, char * value, int size);

unsigned char * serializeClientInfo(unsigned char * buffer, ClientInfo * clientInfo);

unsigned char * serializeConnection(unsigned char * buffer, Connection * connection);

int deserializeInt(unsigned char *buffer);

short deserializeShort(unsigned char * buffer);

char deserializeChar(unsigned char *buffer);

void deserializeCharArray(unsigned char * buffer, char * value);

ClientInfo * deserializeClient(unsigned char * buffer);

Connection * deserializeConnection(unsigned char * buffer);

#endif //SNAKES_SERIALIZE_H
