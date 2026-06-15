/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI game state
*/

#pragma once

#include "Game/Egg.hpp"
#include "Game/Incantation.hpp"
#include "Game/Player.hpp"
#include "Game/ServerMessage.hpp"
#include "Game/Tile.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace GUI {

/*
** Related server commands:
** - msz X Y (map size)
** - bct X Y q0 q1 q2 q3 q4 q5 q6 (tile content)
** - tna N (team name)
** - sgt T (time unit)
** - sst T (time unit modification)
** - seg N (end of game)
** - player, egg and incantation update commands (world state updates)
*/
class GameState {
    public:
        GameState(int mapWidth = 0, int mapHeight = 0, int timeUnit = 0);
        ~GameState() = default;

        std::size_t mapWidth = 0;
        std::size_t mapHeight = 0;
        int timeUnit = 0;
        bool gameEnded = false;
        std::string winningTeam;

        std::vector<Tile> tiles;
        std::unordered_map<int, Player> players;
        std::unordered_map<int, Egg> eggs;
        std::vector<std::string> teams; //? replace by references to a class or smth?
        std::vector<ServerMessage> messages;
        std::vector<Incantation> incantations;
};

}
