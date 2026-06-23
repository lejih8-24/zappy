/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI player state
*/

#include "Game/Player.hpp"

namespace GUI {

Player::Player(int id, int x, int y, std::size_t level, const std::string &teamName)
    : Entity(id, x, y)
    , level(level)
    , teamName(teamName)
{
}

}
