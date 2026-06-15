/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI incantation state
*/

#include "Game/Incantation.hpp"

namespace GUI {

Incantation::Incantation(int id, int x, int y, int level, const std::vector<int> &playerIds)
    : Entity(id, x, y)
    , _level(level)
    , _playerIds(playerIds)
{
}

}
