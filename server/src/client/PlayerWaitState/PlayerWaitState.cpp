/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** PlayerWaitState class
*/

#include "PlayerWaitState.hpp"
#include <zappy/client.hpp>
#include <zappy/game.hpp>


using std::chrono_literals::operator ""ms;


Zappy::Client::PlayerWaitState::PlayerWaitState(Game::Player& player)
    : QueueState()
    , m_Player(player)
{

}

void Zappy::Client::PlayerWaitState::update(Client& client, std::chrono::nanoseconds dt)
{
    m_Player.reduceCooldown(dt);

    if (m_Player.cooldown() >= 0.0ms)
        return;

    client.setState(std::make_unique<PlayerRunState>(m_Player));
}

void Zappy::Client::PlayerWaitState::disconnect(Game::Game& game)
{
    game.killPlayer(m_Player);
}
