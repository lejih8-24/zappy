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


Zappy::Client::PlayerWaitState::PlayerWaitState(Game::Player& player, std::chrono::duration<double, std::milli> period)
    : QueueState()
    , m_Player(player)
    , m_WaitDuration(period)
{

}

void Zappy::Client::PlayerWaitState::update(Client& client, std::chrono::nanoseconds dt)
{
    m_WaitDuration -= dt;

    if (m_WaitDuration >= m_WaitDuration.zero())
        return;

    client.setState(std::make_unique<PlayerRunState>(m_Player));
}
