#include <stdint.h>
#include <stdio.h>
#include <strings.h>
#include "../server/Server.h"
#include "../template/WindowProperties.h"

//
// Created by dylan on 15/04/2017.
//
unsigned char * serializeInt(unsigned char *buffer, int value) {
    // Integer is 4 bytes while char is 1 byte
    buffer[0] = (unsigned char) (value >> 24);
    buffer[1] = (unsigned char) (value >> 16);
    buffer[2] = (unsigned char) (value >> 8);
    buffer[3] = (unsigned char) value;
    return buffer + 4;
}

unsigned char * serializeShort(unsigned char *buffer, short value) {
    buffer[0] = (unsigned char) (value >> 8);
    buffer[1] = (unsigned char) value;
    return buffer + 2;
}

unsigned char * serializeChar(unsigned char *buffer, char value) {
    buffer[0] = (unsigned char) value;
    return buffer + 1;
}

unsigned char * serializeCharArray(unsigned char * buffer, char * value, int size) {
    for (int i = 0; i < size; i++) {
        buffer = serializeChar(buffer, value[0]);
    }
    return buffer;
}

unsigned char * serializeClientInfo(unsigned char *buffer, ClientInfo * clientInfo) {
    buffer = serializeCharArray(buffer, clientInfo->name, MAXIMUM_INPUT_STRING);
    buffer = serializeShort(buffer, clientInfo->isHost);
    return buffer;
}

unsigned char * serializeConnection(unsigned char *buffer, Connection * connection) {
    buffer = serializeInt(buffer, connection->sockFd);
    buffer = serializeClientInfo(buffer, connection->clientInfo);
    return buffer;
}

int deserializeInt(unsigned char *buffer) {
    uint32_t value = 0;

    value |= (uint32_t) buffer[0] << 24;
    value |= (uint32_t) buffer[1] << 16;
    value |= (uint32_t) buffer[2] << 8;
    value |= (uint32_t) buffer[3];
    return (int) value;
}

short deserializeShort(unsigned char *buffer) {
    uint16_t value = 0;
    value |= (uint16_t) buffer[0] << 8;
    value |= (uint16_t) buffer[1];
    return (short) value;
}

char deserializeChar(unsigned char *buffer) {
    return buffer[0];
}

void deserializeCharArray(unsigned char *buffer, char * value) {
    bzero(value, MAXIMUM_INPUT_STRING);

    for (int i = 0; i < MAXIMUM_INPUT_STRING; i++) {
        value[i] = deserializeChar(buffer + i);
    }
}

ClientInfo * deserializeClient(unsigned char *buffer) {
    ClientInfo * client = (ClientInfo *) malloc(sizeof(ClientInfo));

    if (client == NULL) {
        perror("Failed to allocate memory to client");
        return NULL;
    }
    client->isHost = deserializeShort(buffer);
    deserializeCharArray(buffer + 2, client->name);
    client->name[MAXIMUM_INPUT_STRING - 1] = '\0';
    return client;
}

Connection *deserializeConnection(unsigned char *buffer) {
    Connection * connection = (Connection *) malloc(sizeof(Connection));

    if (connection == NULL) {
        perror("Failed to allocate memory to Connection");
        return NULL;
    }

    connection->sockFd = deserializeInt(buffer);
    connection->clientInfo = deserializeClient(buffer + 4);
    return connection;
}
