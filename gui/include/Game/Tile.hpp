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
        ResourceStock resources = {};
        //* TODO: voir si ça serait pas mieux d'avoir des références aux objets??
        std::vector<int> playerIds;
        std::vector<int> eggIds;
        std::vector<int> incantationIds;

    public:
        Tile(int x = 0, int y = 0, const ResourceStock &resources = {});
        ~Tile() = default;
};

}
