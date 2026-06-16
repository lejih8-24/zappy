/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI egg state
*/

#include "Game/Egg.hpp"

namespace GUI {

Egg::Egg(int id, int parentId, int x, int y)
    : Entity(id, x, y)
    , parentPlayerId(parentPlayerId)
{
}

}
