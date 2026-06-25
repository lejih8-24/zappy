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
    struct Resources;

    class Event {
        public:
            static inline std::string mapSize(std::pair<unsigned int, unsigned int> size) { return mapSize(size.first, size.second); }
            static std::string mapSize(unsigned int width, unsigned int height);
            static std::string tileContents(unsigned int x, unsigned int y, const Resources& resources);
            static std::string teamName(std::string_view name);

            static std::string eggNew(int eggId, int playerId, std::pair<unsigned int, unsigned int> pos);
    };
}
