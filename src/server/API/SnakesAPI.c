//
// Created by dylan on 21/04/2017.
//

#include <strings.h>
#include "../../utility/Vector.h"
#include "../Server.h"
#include "../../utility/Serialize.h"
#include "../../utility/General.h"
#include "../ServerHandle.h"
#include <unistd.h>
#include <stdio.h>

bool sendSnakeDataToClients(Vector * connections) {
    int response;
    // For every snake send the direction, size as Int and every position for every snake.
    size_t size = DELIMITERS_SIZE + (connections->size * (INTEGER_BYTES * 2));

    // Calculate the size of positions of the snake.
    for (int i = 0; i < connections->size; i++) {
        size += (((Connection *) connections->data[i])->snake->size) * POSITION_BYTES;
    }

    unsigned char buffer[size];
    bzero(buffer, size);

    serializedSnakesFromConnections(buffer, connections);

    // Write to all the clients about the information.
    for (int i = 0; i < connections->size; i++) {
        connection = (Connection *) connections->data[i];

        response = (int) write(connection->sockFd, buffer, size);

        if (response == -1) {
            perror("Failed to write to the socket");
            // Error, close socket, delete connection and indicate the server to
            // resend the data.
            freeConnection(connection);
            deleteItemFromVector(connections, connection);
            return false;
        }
    }
    return true;
}
