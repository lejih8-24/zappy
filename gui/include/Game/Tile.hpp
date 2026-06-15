/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI tile state
*/

#pragma once

#include "Game/Entity.hpp"
#include "Game/Resource.hpp"

#include <vector>

namespace GUI {

/*
** Related server commands:
** - bct X Y q0 q1 q2 q3 q4 q5 q6 (content of a tile)
*/
class Tile : public Position {
    public:
        Tile(int x = 0, int y = 0, const ResourceStock &resources = {});
        ~Tile() = default;

        const ResourceStock &getResources() const { return _resources; }
        void setResources(const ResourceStock &resources) { _resources = resources; }

        const std::vector<int> &getPlayerIds() const { return _playerIds; }
        void setPlayerIds(const std::vector<int> &playerIds) { _playerIds = playerIds; }

        const std::vector<int> &getEggIds() const { return _eggIds; }
        void setEggIds(const std::vector<int> &eggIds) { _eggIds = eggIds; }

        const std::vector<int> &getIncantationIds() const { return _incantationIds; }
        void setIncantationIds(const std::vector<int> &incantationIds) { _incantationIds = incantationIds; }

    private:
        ResourceStock _resources = {};
        //* TODO: voir si ça serait pas mieux d'avoir des références aux objets??
        std::vector<int> _playerIds;
        std::vector<int> _eggIds;
        std::vector<int> _incantationIds;
};

}
