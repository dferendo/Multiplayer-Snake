//
// Created by dylan on 20/04/2017.
//

#include <strings.h>
#include <unistd.h>
#include <memory.h>
#include <errno.h>
#include "../../settings/GameSettings.h"
#include "../../utility/Serialize.h"
#include "../../utility/General.h"


void writeNameToSocket(int socketFileDescriptor, char * name) {
    int response;
    unsigned char buffer[MAXIMUM_INPUT_STRING];
    bzero(buffer, MAXIMUM_INPUT_STRING);

    serializeCharArray(buffer, name, MAXIMUM_INPUT_STRING);
    response = (int) write(socketFileDescriptor, buffer, MAXIMUM_INPUT_STRING);

    if (response == -1) {
        perror("Error reading from socket");
        close(socketFileDescriptor);
        exit(1);
    }
}

bool checkIfNameWasAccepted(int sockFileDescriptor) {
    int response;
    unsigned char buffer[DELIMITERS_SIZE];

    bzero(buffer, DELIMITERS_SIZE);

    response = (int) read(sockFileDescriptor, buffer, DELIMITERS_SIZE);

    if (response < 0) {
        perror("Error when reading from file");
        exit(1);
    }
    // Check if name was accepted.
    if (strncmp((const char *) buffer, NAME_ACCEPTED, DELIMITERS_SIZE) == 0) {
        return true;
    } else if (strncmp((const char *) buffer, NAME_NOT_ACCEPTED, DELIMITERS_SIZE) == 0) {
        false;
    } else {
        perror("Wrong data read, problem with parallelism from server and client.");
        exit(1);
    }
    return false;
}

void writeStartGameToSocket(int *sockFd) {
    int response;
    unsigned char buffer[DELIMITERS_SIZE];
    bzero(buffer, DELIMITERS_SIZE);

    serializeCharArray(buffer, HOST_STARTS_GAME_DELIMITER, DELIMITERS_SIZE);
    response = (int) write(*sockFd, buffer, DELIMITERS_SIZE);

    if (response == -1) {
        perror("Error reading from socket");
        close(*sockFd);
        exit(1);
    }
}

int readDelimiterQueue(int *sockFd) {
    int response;
    unsigned char buffer[DELIMITERS_SIZE];

    bzero(buffer, DELIMITERS_SIZE);

    setSocketBlockingEnabled(*sockFd, false);
    response = (int) read(*sockFd, buffer, DELIMITERS_SIZE);
    setSocketBlockingEnabled(*sockFd, true);

    if (response < 0) {
        // Since not blocking ignore this error
        if (errno == EAGAIN) {
            return 0;
        }
        perror("Error reading to socket");
        exit(1);
    }

    if (strncmp((const char *) buffer, HOST_STARTS_GAME_DELIMITER, DELIMITERS_SIZE) == 0) {
        return 1;
    } else if (strncmp((const char *) buffer, VECTOR_OF_CONNECTIONS_DELIMITER, DELIMITERS_SIZE) == 0) {
        return 2;
    } else {
        return -2;
    }
}

Vector * readConnectionsFromSocket(int socketFileDescriptor) {
    int response;
    int amountOfConnections;
    size_t connectionsSize;
    unsigned char bufferInteger[INTEGER_BYTES];
    bzero(bufferInteger, INTEGER_BYTES);
    Vector * connections = initVector();

    response = (int) read(socketFileDescriptor, bufferInteger, INTEGER_BYTES);

    if (response == -1) {
        perror("Error when reading from socket");
        close(socketFileDescriptor);
        return NULL;
    }

    deserializeInt(bufferInteger, &amountOfConnections);
    connectionsSize = (size_t) (amountOfConnections * CONNECTION_BYTES_NO_SNAKE);

    unsigned char buffer[connectionsSize];
    bzero(buffer, connectionsSize);

    response = (int) read(socketFileDescriptor, buffer, connectionsSize);

    if (response == -1) {
        perror("Error when reading from socket");
        close(socketFileDescriptor);
        exit(1);
    }
    // De-serialize the connections and put them in a vector
    deserializeVectorOfConnections(buffer, connections, amountOfConnections);
    return connections;
}