#include "Serialize.h"
#include "../server/Food.h"
#include "../server/ServerHandle.h"

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

unsigned char * serializeCharArray(unsigned char * buffer, char * value, int size) {
    for (int i = 0; i < size; i++) {
        buffer[i] = (unsigned char) value[i];
    }
    return buffer + size;
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

unsigned char *serializedSnakeDirectionWithDelimiter(unsigned char *buffer, int direction) {
    buffer = serializeCharArray(buffer, CHANGE_DIRECTION_DELIMITER, DELIMITERS_SIZE);
    buffer = serializeInt(buffer, direction);
    return buffer;
}

unsigned char * serializedSnakesFromConnections(unsigned char *buffer, Vector *connections) {
    Connection * connection;
    // Add delimiter
    buffer = serializeCharArray(buffer, SNAKE_DETAILS_DELIMITER, DELIMITERS_SIZE);
    // Send the amount of snakes
    buffer = serializeInt(buffer, (int) connections->size);

    // After the delimiter there will be the size of the first snake. This will help calculate
    // the next position of the size of the next snake.
    for (int i = 0; i < connections->size; i++) {
        connection = (Connection *) connections->data[i];
        buffer = serializeInt(buffer, connection->snake->size);
        buffer = serializedSnake(buffer, connection->snake);
    }
    return buffer;
}

unsigned char *serializedUserUniqueID(unsigned char *buffer, int uniqueID) {
    buffer = serializeCharArray(buffer, UNIQUE_ID_DELIMITER, DELIMITERS_SIZE);
    buffer = serializeInt(buffer, uniqueID);
    return buffer;
}
