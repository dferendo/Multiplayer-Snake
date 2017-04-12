cmake_minimum_required(VERSION 3.7)
project(Snakes)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES src/main.c
        src/snakes-components/GameBoard.h
        src/snakes-components/GameBoard.c
        src/snakes-components/Food.h
        src/snakes-components/Food.c
        src/snakes-components/Snake.h
        src/snakes-components/Snake.c src/snakes-components/Game.h src/snakes-components/Game.c src/Utility/LinkedList.h src/Utility/LinkedList.c src/Utility/Vector.h src/Utility/Vector.c)
add_executable(Snakes ${SOURCE_FILES})
# Add ncurses library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})
target_link_libraries( Snakes ${CURSES_LIBRARIES})