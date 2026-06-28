/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Parser.hpp
*/

#pragma once

#include "Game/GameState.hpp"
#include <string>

namespace GUI
{
    class Parser {
        public:
            Parser(GameState &state);
            void parseLine(const std::string &line);
        private:
            GameState &_state;

            void parseMsz(const std::string &line);
            void parseTna(const std::string &line);
            void parsePnw(const std::string &line);
            void parsePpo(const std::string &line);
            void parsePlv(const std::string &line);
    };
}
