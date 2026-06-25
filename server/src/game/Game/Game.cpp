/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Game class
*/

#include "Game.hpp"
#include <zappy/game.hpp>
#include <random>


std::mt19937 Zappy::Game::Game::s_RNG(std::random_device{}());


Zappy::Game::Game::Game() noexcept
    : m_Map(10, 10)
    , m_Players()
    , m_Eggs()
    , m_Teams()
    , m_GraphicsEvents()
    , m_GameSpeed(1)
{

}

void Zappy::Game::Game::update(std::chrono::nanoseconds dt)
{
    m_GraphicsEvents.clear();
}

void Zappy::Game::Game::setTeams(std::span<std::string> names, std::size_t maxMembers)
{
    m_Teams.clear();

    for (auto& name : names) {
        Team team(name, maxMembers);

        for (std::size_t i = 0; i < maxMembers; i++) {
            auto [randX, randY] = randomPosition();
            m_Eggs.emplace_back(-1, name, randX, randY);
        }

        m_Teams[std::move(name)] = std::move(team);
    }
}

void Zappy::Game::Game::playerLayEgg(const Player& player)
{
    auto [x, y] = player.position();
    auto& egg = m_Eggs.emplace_back(player.id(), player.team(), x, y);
    m_GraphicsEvents.push_back(Event::eggNew(egg.id(), egg.parentId(), egg.getPosition()));
}
