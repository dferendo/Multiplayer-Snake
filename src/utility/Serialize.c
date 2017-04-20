#include "Serialize.h"
#include "../server/Food.h"
#include <strings.h>
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

unsigned char * serializeVectorOfConnections(unsigned char *buffer, Vector *connections) {
    buffer = serializeInt(buffer, (int) connections->size);
    // Fill the buffer with data
    for (int i = 0; i < connections->size; i++) {
        buffer = serializeConnection(buffer, (Connection *) connections->data[i]);
    }
    return buffer;
}

unsigned char * serializedVectorOfConnectionsDelimiter(unsigned char *buffer, Vector *connections) {
    buffer = serializeCharArray(buffer, VECTOR_OF_CONNECTIONS_DELIMITER, DELIMITERS_SIZE);
    buffer = serializeVectorOfConnections(buffer, connections);
    return buffer;
}

unsigned char * serializedSnake(unsigned char *buffer, Snake *snake) {
    buffer = serializeInt(buffer, snake->direction);
    buffer = serializedLinkedList(buffer, snake->positions);
    return buffer;
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

unsigned char * deserializeCharArray(unsigned char *buffer, char *value, int size) {

    for (int i = 0; i < size; i++) {
        value[i] = buffer[i];
    }
    return buffer + size;
}

unsigned char * deserializeClientInfo(unsigned char *buffer, ClientInfo *clientInfo) {
    buffer = deserializeCharArray(buffer, clientInfo->name, MAXIMUM_INPUT_STRING);
    buffer = deserializeShort(buffer, &clientInfo->isHost);
    clientInfo->snake = NULL;
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
    buffer = deserializeClientInfo(buffer, client);
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

unsigned char * serializedLinkedList(unsigned char *buffer, LinkedListPosition *linkedListPosition) {

    while (true) {
        // Assumed the linked list will at least have 1 size.
        buffer = serializedPosition(buffer, linkedListPosition->position);
        if (linkedListPosition->next == NULL) {
            return buffer;
        }
        linkedListPosition = linkedListPosition->next;
    }
}

unsigned char * serializedPosition(unsigned char *buffer, Position *position) {
    buffer = serializeInt(buffer, position->x);
    buffer = serializeInt(buffer, position->y);
    return buffer;
}

unsigned char * serializedSnakeFromConnections(unsigned char *buffer, Vector *connections) {
    Connection * connection;
    // Add delimiter
    buffer = serializeCharArray(buffer, SNAKE_DETAILS_DELIMITER, DELIMITERS_SIZE);

    for (int i = 0; i < connections->size; i++) {
        connection = (Connection *) connections->data[i];
        // Send the owner of the snake before.
        buffer = serializeCharArray(buffer, connection->clientInfo->name, MAXIMUM_INPUT_STRING);
        buffer = serializeInt(buffer, connection->clientInfo->snake->size);
        buffer = serializedSnake(buffer, connection->clientInfo->snake);
    }
    return buffer;
}

unsigned char * serializedVectorOfFoodsWithDelimiter(unsigned char *buffer, Vector *foods) {
    buffer = serializeCharArray(buffer, VECTOR_OF_FOOD_DELIMITER, DELIMITERS_SIZE);
    // Put amount of food after delimiter
    buffer = serializeInt(buffer, (int) foods->size);

    for (int i = 0; i < foods->size; i++) {
        Food * food = (Food *) foods->data[i];
        // Serialize positions
        buffer = serializedPosition(buffer, food->position);
        // Serialize Type
        buffer = serializeInt(buffer, food->foodType);
    }
    return buffer;
}

unsigned char * deserializedPosition(unsigned char *buffer, Position *position) {
    buffer = deserializeInt(buffer, &position->x);
    buffer = deserializeInt(buffer, &position->y);
    return buffer;
}

unsigned char * deserializedVectorOfFoods(unsigned char *buffer, Vector *foods, int size) {
    Food * food;
    Position * position;
    int type;

    for (int i = 0; i < size; i++) {
        food = (Food *) malloc(sizeof(Food));

        if (food == NULL) {
            perror("Failed to allocate memory to Food");
            return buffer;
        }
        position = (Position *) malloc(sizeof(Position));

        if (position == NULL) {
            perror("Failed to allocate memory to Position.");
            free(food);
            return buffer;
        }

        buffer = deserializedPosition(buffer, position);
        type = (int) food->foodType;
        buffer = deserializeInt(buffer, &type);
        food->foodType = (Type) type;
        food->position = position;
        addItemToVector(foods, food);
    }
    return buffer;
}

unsigned char * deserializedSnake(unsigned char *buffer, Snake * snake, int size) {
    Position * position;
    int direction;

    snake->size = size;
    buffer = deserializeInt(buffer, &direction);
    snake->direction = (Direction) direction;

    // Deserialize positions of snakes
    for (int i = 0; i < size; i++) {
        position = (Position *) malloc(sizeof(Position));

        if (position == NULL) {
            perror("Failed to allocate memory to Position");
            return buffer;
        }

        buffer = deserializedPosition(buffer, position);
        if (i == 0) {
            snake->positions = initLinkedListPosition(position);
        } else {
            addPosition(snake->positions, position);
        }
    }
    return buffer;
}

unsigned char * deserializedNameAndSizeOfSnake(unsigned char *buffer, char *name, int *size) {
    buffer = deserializeCharArray(buffer, name, MAXIMUM_INPUT_STRING);
    buffer = deserializeInt(buffer, size);
    return buffer;
}

unsigned char *serializedSnakeDirectionWithDelimiter(unsigned char *buffer, int direction) {
    buffer = serializeCharArray(buffer, CHANGE_DIRECTION_DELIMITER, DELIMITERS_SIZE);
    buffer = serializeInt(buffer, direction);
    return buffer;
}
