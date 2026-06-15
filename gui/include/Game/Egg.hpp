/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI egg state
*/

#pragma once

#include "Game/Entity.hpp"

namespace GUI {

/*
** Related server commands:
** - enw #e #n X Y (new egg)
** - ebo #e (egg hatch)
** - edi #e (egg death)
*/
class Egg : public Entity {
    public:
        Egg(int id = 0, int playerId = 0, int x = 0, int y = 0);
        ~Egg() = default;

        int playerId = 0;
        bool alive = true;
};

}
