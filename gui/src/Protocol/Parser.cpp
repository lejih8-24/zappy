/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Parser.cpp
*/

#include "Protocol/Parser.hpp"
#include <sstream>

namespace GUI {
    Parser::Parser(GameState &state) : _state(state)
    {

    }

    void Parser::parseLine(const std::string &line)
    {
        if (line.substr(0, 3) == "msz")
            parseMsz(line);
        else if (line.substr(0, 3) == "tna")
            parseTna(line);
        else if (line.substr(0, 3) == "pnw")
            parsePnw(line);
        else if (line.substr(0, 3) == "ppo")
            parsePpo(line);
        else if (line.substr(0, 3) == "plv")
            parsePlv(line);
    }

    void Parser::parseMsz(const std::string &line)
    {
        std::istringstream ss(line);
        std::string cmd;
        int w, h;
        ss >> cmd >> w >> h;
        _state.mapWidth = w;
        _state.mapHeight = h;
    }

    void Parser::parseTna(const std::string &line)
    {
        std::istringstream ss(line);
        std::string cmd, name;
        ss >> cmd >> name;
        _state.teams.push_back(name);
    }

    void Parser::parsePnw(const std::string &line)
    {
        std::istringstream ss(line);
        std::string cmd, id, team;
        int x, y, orientation, level;
        ss >> cmd >> id >> x >> y >> orientation >> level >> team;
        int playerId = std::stoi(id.substr(1));
        _state.players.emplace(playerId, Player(playerId, x, y, level, team));
        
    }

    void Parser::parsePpo(const std::string &line)
    {
        std::istringstream ss(line);
        std::string cmd, id;
        int x, y, orientation;
        ss >> cmd >> id >> x >> y >> orientation;
        int playerId = std::stoi(id.substr(1));
        if (_state.players.count(playerId)) {
            _state.players[playerId].x = x;
            _state.players[playerId].y = y;
        }
    }

    void Parser::parsePlv(const std::string &line)
    {
        std::istringstream ss(line);
        std::string cmd, id;
        int level;
        ss >> cmd >> id >> level;
        int playerId = std::stoi(id.substr(1));
        if (_state.players.count(playerId)) {
            _state.players[playerId].level = level;
        }
    }

}