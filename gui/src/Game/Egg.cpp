/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI egg state
*/

#include "Game/Egg.hpp"

namespace GUI {

Egg::Egg(int id, int playerId, int x, int y)
    : Entity(id, x, y)
    , playerId(playerId)
{
}

}
