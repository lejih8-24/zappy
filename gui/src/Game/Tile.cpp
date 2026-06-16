/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI tile state
*/

#include "Game/Tile.hpp"

namespace GUI {

Tile::Tile(int x, int y, const ResourceStock &resources)
    : Position{x, y}
    , resources(resources)
{
}

}
