/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI identified state
*/

#include "Game/Entity.hpp"

namespace GUI {

Entity::Entity(int id, int x, int y)
    : Position{x, y}
    , id(id)
{
}

}
