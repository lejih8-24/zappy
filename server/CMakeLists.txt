##
## EPITECH PROJECT, 2026
## Project - Zappy
## File description:
## Server build file
##

cmake_minimum_required(VERSION 3.15)

add_subdirectory(../lib/liblattice "${CMAKE_BINARY_DIR}/lib/liblattice")

add_executable(zappy_server
               src/exceptions/ArgumentParseException/ArgumentParseException.cpp
               src/Server/Server.cpp
               src/main.cpp)

target_include_directories(zappy_server
                           PRIVATE include
                           PRIVATE ../lib/liblattice/include)

target_link_libraries(zappy_server PRIVATE lattice)
