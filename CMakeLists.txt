cmake_minimum_required(VERSION 3.7)
project(Snakes)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES library.c library.h)
add_library(Snakes ${SOURCE_FILES})