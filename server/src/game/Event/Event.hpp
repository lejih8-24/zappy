/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Event class
*/

#pragma once

#include <string>


namespace Zappy::Game {
    class Event {
        public:
            static std::string mapSize(unsigned int width, unsigned int height);
            static std::string eggNew(int eggId, int playerId, std::pair<unsigned int, unsigned int> pos);
    };
}
