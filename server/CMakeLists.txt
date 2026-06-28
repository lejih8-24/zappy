##
## EPITECH PROJECT, 2026
## Project - Zappy
## File description:
## Server build file
##

cmake_minimum_required(VERSION 3.15)

add_executable(zappy_server
               src/client/Client/Client.cpp
               src/client/GuiState/GuiState.cpp
               src/client/HandshakeState/HandshakeState.cpp
               src/client/PlayerInitState/PlayerInitState.cpp
               src/client/PlayerRunState/PlayerRunState.cpp
               src/client/PlayerWaitState/PlayerWaitState.cpp
               src/exceptions/ArgumentParseException/ArgumentParseException.cpp
               src/game/Egg/Egg.cpp
               src/game/Event/Event.cpp
               src/game/Game/Game.cpp
               src/game/Player/Player.cpp
               src/game/Resources/Resources.cpp
               src/game/Team/Team.cpp
               src/Server/Server.cpp
               src/utils/Logger/Logger.cpp
               src/main.cpp)

target_include_directories(zappy_server
                           PRIVATE include
                           PRIVATE ../lib/liblattice/include)

target_link_libraries(zappy_server PRIVATE lattice)
