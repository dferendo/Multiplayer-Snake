
#include <ncurses.h>
#include "template/ClientLayout.h"
#include "ConnectMenu.h"

int main(int argc, char *argv[]) {
    // Check if port and hostname were passed.
    if (argc < 3) {
        printf("Not enough arguments passed. Port and host required\n");
        exit(0);
    }
    ncursesInit();
    // Send port number and hostname to server.
    serverConnection(atoi(argv[2]), argv[1]);
    // Stop display.
    endwin();
    return 0;
}