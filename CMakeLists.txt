cmake_minimum_required(VERSION 3.7)
project(Snakes)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pthread")

set(CLIENT_FILES
        src/utility/Vector.c
        src/utility/General.c
        src/utility/Serialize.c
        src/template/ClientLayout.c
        src/client/Client.c
        src/client/StartGame.c)

set(SERVER_FILES
        src/utility/Vector.c
        src/utility/Serialize.c
        src/server/Server.c
        src/server/QueueToPlay.c)

add_executable(Client ${CLIENT_FILES})
add_executable(Server ${SERVER_FILES})

# Add ncurses library
find_package(Curses  REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})
target_link_libraries( Client ${CURSES_LIBRARIES})