CC=gcc
GCCFLAGS=-Wall -Werror
LIBS=-lncurses -pthread
RM=rm -f

.PHONY: clean

SRC_CLIENT=src/client/*.c src/client/API/ClientAPI.c src/client/template/ClientLayout.c src/utility/*.c src/server/Snake.c

SRC_SERVER=src/server/*.c src/server/API/*.c src/utility/*.c 

all: Client Server

Client:
	$(CC) $(GCCFLAGS) -o Client $(SRC_CLIENT) $(LIBS)

Server:
	$(CC) $(GCCFLAGS) -o Server $(SRC_SERVER) $(LIBS)

clean:
	$(RM) Client Server
