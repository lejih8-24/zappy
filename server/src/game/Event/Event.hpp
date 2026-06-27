/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Event class
*/

#pragma once

#include <string>
#include <span>


namespace Zappy::Game {
    enum class Orientation;
    struct Resources;
    class Player;

    class Event {
        public:
            static inline std::string mapSize(std::pair<unsigned int, unsigned int> size) { return mapSize(size.first, size.second); }
            static std::string mapSize(unsigned int width, unsigned int height);
            static std::string tileContents(unsigned int x, unsigned int y, const Resources& resources);
            static std::string teamName(std::string_view name);

            static std::string playerNew(int playerId, std::pair<unsigned int, unsigned int> pos, Orientation orientation, int level, std::string_view team);
            static std::string playerPosition(int playerId, std::pair<unsigned int, unsigned int> pos, Orientation orientation);
            static std::string playerLevel(int playerId, unsigned int level);
            static std::string playerInventory(int playerId, std::pair<unsigned int, unsigned int> pos, const Resources& inventory);
            static std::string playerDie(int playerId);
            static std::string playerCollect(int playerId, unsigned int resource);
            static std::string playerDrop(int playerId, unsigned int resource);
            static std::string playerIncantationStart(std::pair<unsigned int, unsigned int> pos, unsigned int level, std::span<Player*> players);
            static std::string playerIncantationStart(std::pair<unsigned int, unsigned int> pos, unsigned int level, std::string_view playerIds);

            static std::string eggNew(int eggId, int playerId, std::pair<unsigned int, unsigned int> pos);
            static std::string eggHatch(int eggId);

            static std::string serverGameSpeed(unsigned int speed);
    };
}
