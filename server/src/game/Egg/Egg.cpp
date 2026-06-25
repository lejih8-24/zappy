/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Egg class
*/

#include "Egg.hpp"


int Zappy::Game::Egg::s_NextId = 0;


Zappy::Game::Egg::Egg(int playerId, std::string_view team, unsigned int tileX, unsigned int tileY) noexcept
    : m_Id(s_NextId)
    , m_PlayerId(playerId)
    , m_TeamName(team)
    , m_Position{ tileX, tileY }
{
    s_NextId++;
}
