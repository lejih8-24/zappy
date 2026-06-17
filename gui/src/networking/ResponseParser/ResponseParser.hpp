/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** ResponseParser class
*/

#pragma once

#include <game.hpp>


namespace Zappy::Networking {
    using Coordinate = unsigned int;

    struct Position {
        Coordinate x;
        Coordinate y;
    };

    struct TileContents {
        Position position;
        Game::Resources resources;
    };

    struct PlayerPosition {
        unsigned int playerId;
        Position position;
        float rotationDeg;  // player rotation in degrees
    };

    class ResponseParser {
        public:
            static Position parseMapSize(std::string_view response);
            static TileContents parseTileContents(std::string_view response);

        private:
            static std::pair<std::string_view, std::string_view> splitWord(std::string_view line);
            static std::string_view extractWord(std::string_view& line);
            static unsigned int toInteger(std::string_view repr);
    };
}
