/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Player class
*/

#include "Player.hpp"


int Zappy::Game::Player::s_NextId = 0;


Zappy::Game::Player::Player(std::string_view team)
    : m_Id(s_NextId)
    , m_TeamName(team)
    , m_Position{ 0, 0 }
{
    s_NextId++;
}
