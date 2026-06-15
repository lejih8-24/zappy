/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI player state
*/

#pragma once

#include "Game/Entity.hpp"
#include "Game/Resource.hpp"

#include <string>

namespace GUI {

enum class Orientation {
    North = 1,
    East = 2,
    South = 3,
    West = 4,
};

/*
** Related server commands:
** - pnw #n X Y O L N (new player)
** - ppo #n X Y O (player position)
** - plv #n L (player level)
** - pin #n X Y q0 q1 q2 q3 q4 q5 q6 (player inventory)
** - pex #n (player expulsion)
** - pbc #n M (player broadcast)
** - pfk #n (egg laying start)
** - pdr #n i (resource drop)
** - pgt #n i (resource collect)
** - pdi #n (player death)
*/
class Player : public Entity {
    public:
        Player(int id = 0, int x = 0, int y = 0, Orientation orientation = Orientation::North, std::size_t level = 1,
            const std::string &teamName = "");
        ~Player() = default;

        Orientation orientation = Orientation::North;
        std::size_t level = 1;
        std::string teamName;
        ResourceStock inventory = {};
        bool alive = true;
        bool isBroadcasting = false;
        bool isIncantating = false;
        bool isLayingEgg = false;
};

}
