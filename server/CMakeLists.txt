##
## EPITECH PROJECT, 2026
## Project - Zappy
## File description:
## Server build file
##

cmake_minimum_required(VERSION 3.15)

add_executable(zappy_server
               src/networking/NonBlockingSocket/NonBlockingSocket.cpp
               src/networking/Socket/Socket.cpp
               src/main.cpp)

target_include_directories(zappy_server
                           PRIVATE include)
