##
## EPITECH PROJECT, 2026
## Project - Zappy
## File description:
## Main project build file
##

cmake_minimum_required(VERSION 3.15)

project(Zappy VERSION 1.0
              DESCRIPTION "Zappy Project - 2026"
              HOMEPAGE_URL "https://github.com/LeJih8-24/Zappy"
              LANGUAGES CXX)

# Project-wide C++ Standard
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(lib/liblattice)
add_subdirectory(ai)
add_subdirectory(gui)
add_subdirectory(server)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(tests)
endif()
