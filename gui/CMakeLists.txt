##
## EPITECH PROJECT, 2026
## Project - Zappy
## File description:
## Client build file
##

cmake_minimum_required(VERSION 3.15)

add_executable(zappy_gui
               src/main.cpp)

target_include_directories(zappy_gui
                           PRIVATE include)
