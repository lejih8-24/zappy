/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Player class
*/

#include "Player.hpp"
#include <zappy/game.hpp>


int Zappy::Game::Player::s_NextId = 0;


Zappy::Game::Player::Player() noexcept
    : m_Id(-1)
    , m_TeamName()
    , m_Position{ 0, 0 }
    , m_Orientation(Orientation::NORTH)
    , m_Level(0)
    , m_Inventory{ 0 }
    , m_TimeToLive(0)
{

}

Zappy::Game::Player::Player(std::string_view team)
    : m_Id(s_NextId)
    , m_TeamName(team)
    , m_Position{ 0, 0 }
    , m_Orientation(Orientation::NORTH)
    , m_Level(1)
    , m_Inventory{ 0 }
    , m_TimeToLive(0)
{
    s_NextId++;
    m_Inventory.food = INITIAL_PLAYER_FOOD;
}

void Zappy::Game::Player::moveForward(std::pair<unsigned int, unsigned int> size)
{
    auto  [maxX, maxY] = size;
    auto& [x, y] = m_Position;

    switch (m_Orientation) {
        case Orientation::NORTH: y = (y + maxY - 1) % maxY; return;
        case Orientation::SOUTH: y = (y + 1) % maxY; return;
        case Orientation::EAST:  x = (x + 1) % maxX; return;
        case Orientation::WEST:  x = (x + maxX - 1) % maxX; return;
    };
}

void Zappy::Game::Player::turnRight()
{
    switch (m_Orientation) {
        case Orientation::NORTH: m_Orientation = Orientation::EAST;  return;
        case Orientation::EAST:  m_Orientation = Orientation::SOUTH; return;
        case Orientation::SOUTH: m_Orientation = Orientation::WEST;  return;
        case Orientation::WEST:  m_Orientation = Orientation::NORTH; return;
    }
}

void Zappy::Game::Player::turnLeft()
{
    switch (m_Orientation) {
        case Orientation::NORTH: m_Orientation = Orientation::WEST;  return;
        case Orientation::WEST:  m_Orientation = Orientation::SOUTH; return;
        case Orientation::SOUTH: m_Orientation = Orientation::EAST;  return;
        case Orientation::EAST:  m_Orientation = Orientation::NORTH; return;
    }
}
