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

        std::size_t getMapWidth() const { return _mapWidth; }
        void setMapWidth(std::size_t mapWidth) { _mapWidth = mapWidth; }

        std::size_t getMapHeight() const { return _mapHeight; }
        void setMapHeight(std::size_t mapHeight) { _mapHeight = mapHeight; }

        int getTimeUnit() const { return _timeUnit; }
        void setTimeUnit(int timeUnit) { _timeUnit = timeUnit; }

        bool isGameEnded() const { return _gameEnded; }
        void setGameEnded(bool gameEnded) { _gameEnded = gameEnded; }

        const std::string &getWinningTeam() const { return _winningTeam; }
        void setWinningTeam(const std::string &winningTeam) { _winningTeam = winningTeam; }

        const std::vector<Tile> &getTiles() const { return _tiles; }
        std::vector<Tile> &getTiles() { return _tiles; }

        const std::unordered_map<int, Player> &getPlayers() const { return _players; }
        std::unordered_map<int, Player> &getPlayers() { return _players; }

        const std::unordered_map<int, Egg> &getEggs() const { return _eggs; }
        std::unordered_map<int, Egg> &getEggs() { return _eggs; }

        const std::vector<std::string> &getTeams() const { return _teams; }
        std::vector<std::string> &getTeams() { return _teams; }

        const std::vector<ServerMessage> &getMessages() const { return _messages; }
        std::vector<ServerMessage> &getMessages() { return _messages; }

        const std::vector<Incantation> &getIncantations() const { return _incantations; }
        std::vector<Incantation> &getIncantations() { return _incantations; }

    private:
        std::size_t _mapWidth = 0;
        std::size_t _mapHeight = 0;
        int _timeUnit = 0;
        bool _gameEnded = false;
        std::string _winningTeam;

        std::vector<Tile> _tiles;
        std::unordered_map<int, Player> _players;
        std::unordered_map<int, Egg> _eggs;
        std::vector<std::string> _teams; //? replace by references to a class or smth?
        std::vector<ServerMessage> _messages;
        std::vector<Incantation> _incantations;
};

}
