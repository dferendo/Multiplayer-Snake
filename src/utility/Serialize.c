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

unsigned char * serializeCharArray(unsigned char * buffer, char * value, int size) {
    for (int i = 0; i < size; i++) {
        buffer[i] = (unsigned char) value[i];
    }
    return buffer + size;
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

void serializeVectorOfConnections(unsigned char *buffer, Vector *connections) {
    buffer = serializeInt(buffer, (int) connections->size);
    // Fill the buffer with data
    for (int i = 0; i < connections->size; i++) {
        buffer = serializeConnection(buffer, (Connection *) connections->data[i]);
    }
}

unsigned char * deserializeInt(unsigned char *buffer, int * value) {
    int tempValue = 0;

    tempValue |= buffer[0] << 24;
    tempValue |= buffer[1] << 16;
    tempValue |= buffer[2] << 8;
    tempValue |= buffer[3];
    *value = tempValue;
    return buffer + 4;
}

unsigned char * deserializeShort(unsigned char *buffer, short *value) {
    uint16_t tempValue = 0;

    tempValue |= (uint16_t) buffer[0] << 8;
    tempValue |= (uint16_t) buffer[1];
    *value = tempValue;
    return buffer + 2;
}

unsigned char *deserializeCharArray(unsigned char *buffer, char *value, int size) {

    for (int i = 0; i < size; i++) {
        value[i] = buffer[i];
    }
    return buffer + size;
}

unsigned char * deserializeClient(unsigned char *buffer, ClientInfo * clientInfo) {
    buffer = deserializeCharArray(buffer, clientInfo->name, MAXIMUM_INPUT_STRING);
    buffer = deserializeShort(buffer, &clientInfo->isHost);
    return buffer;
}

unsigned char * deserializeConnection(unsigned char *buffer, Connection *connection) {
    ClientInfo * client = (ClientInfo *) malloc(sizeof(ClientInfo));

    if (client == NULL) {
        perror("Failed to allocate memory to client");
        return buffer;
    }
    // Clear name.
    bzero(client->name, MAXIMUM_INPUT_STRING);

    buffer = deserializeInt(buffer, &connection->sockFd);
    buffer = deserializeClient(buffer, client);
    connection->clientInfo = client;
    return buffer;
}

unsigned char * deserializeVectorOfConnections(unsigned char *buffer, Vector * connections, int size) {

    for (int i = 0; i < size; i++) {
        Connection * connection = (Connection *) malloc(sizeof(Connection));

        if (connection == NULL) {
            perror("Failed to allocate memory to connection");
            return buffer;
        }
        buffer = deserializeConnection(buffer, connection);
        addItemToVector(connections, connection);
    }
    return buffer;
}
