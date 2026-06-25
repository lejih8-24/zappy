/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Team class
*/

#include "Team.hpp"


Zappy::Game::Team::Team() noexcept
    : m_Name()
    , members(0)
    , maxMembers(0)
{

}

Zappy::Game::Team::Team(std::string_view name, std::size_t maxMembers)
    : m_Name(name)
    , members(0)
    , maxMembers(maxMembers)
{

}
