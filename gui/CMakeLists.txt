##
## EPITECH PROJECT, 2026
## Project - Zappy
## File description:
## Client build file
##

cmake_minimum_required(VERSION 3.15)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/raylib     ${CMAKE_BINARY_DIR}/lib/raylib)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/liblattice ${CMAKE_BINARY_DIR}/lib/liblattice)

add_executable(zappy_gui
               src/game/Resources/Resources.cpp
               src/networking/GraphicsClient/GraphicsClient.cpp
               src/networking/ResponseParser/ResponseParser.cpp
               src/Game/Egg.cpp
               src/Game/Entity.cpp
               src/Game/GameState.cpp
               src/Game/GameStateEventHandler.cpp
               src/Game/Incantation.cpp
               src/Game/Player.cpp
               src/Game/ServerMessage.cpp
               src/Game/Tile.cpp
               src/Game/CharacterModel.cpp
               src/Game/EggModel.cpp
               src/Camera.cpp
               src/GuiArgs.cpp
               src/Map.cpp
               src/Render.cpp
               src/Window.cpp
               src/main.cpp)

target_include_directories(zappy_gui
                           PRIVATE include
                           PRIVATE ../lib/liblattice/include)

target_link_libraries(zappy_gui PRIVATE raylib
                                PRIVATE lattice)

target_compile_definitions(zappy_gui PRIVATE ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
                                           PACKS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/packs/")