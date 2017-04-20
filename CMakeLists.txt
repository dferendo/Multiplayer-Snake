cmake_minimum_required(VERSION 3.7)
project(Snakes)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pthread")

set(CLIENT_FILES
        src/utility/Vector.c
        src/utility/General.c
        src/utility/Serialize.c
        src/template/ClientLayout.c
        src/client/Client.c
        src/client/QueueMenu.c
        src/utility/Vector.h
        src/utility/General.h
        src/utility/Serialize.h
        src/template/ClientLayout.h
        src/client/Client.h
        src/client/QueueMenu.h
        src/client/SnakesGame.h
        src/client/SnakesGame.c
        src/template/GameSettings.h
        src/utility/LinkedList.c
        src/utility/LinkedList.h)

set(SERVER_FILES
        src/utility/Vector.c
        src/utility/Serialize.c
        src/server/Server.c
        src/server/QueueToPlay.c
        src/utility/Vector.h
        src/utility/Serialize.h
        src/server/Server.h
        src/server/QueueToPlay.h
        src/server/Snake.h
        src/server/Snake.c
        src/template/GameSettings.h
        src/utility/RandomUtility.c
        src/utility/RandomUtility.h
        src/utility/LinkedList.c
        src/utility/LinkedList.h
        src/utility/General.c
        src/utility/General.h
        src/server/Game.h
        src/server/Game.c
        src/server/Food.h
        src/server/Food.c
        src/server/SnakeMove.h
        src/server/SnakeMove.c)

add_executable(Client ${CLIENT_FILES})
add_executable(Server ${SERVER_FILES})

# Add ncurses library
find_package(Curses  REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})
target_link_libraries( Client ${CURSES_LIBRARIES})