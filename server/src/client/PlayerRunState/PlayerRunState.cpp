/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** PlayerRunState class
*/

#include "PlayerRunState.hpp"
#include <zappy/client.hpp>
#include <zappy/game.hpp>


Zappy::Client::PlayerRunState::PlayerRunState(const std::string& teamName)
    : QueueState()
    , m_TeamName(teamName)
    , m_Player(nullptr)
{

}

Zappy::Client::PlayerRunState::PlayerRunState(std::string&& teamName)
    : QueueState()
    , m_TeamName(std::move(teamName))
    , m_Player(nullptr)
{

}

void Zappy::Client::PlayerRunState::init()
{
    m_Player = game().hatchEgg(m_TeamName);
    if (!m_Player)
        return;

    auto& team = game().teams().at(m_TeamName);
    auto [x, y] = game().mapSize();

    queueMessage(std::to_string(team.maxMembers - team.members) + "\n");
    queueMessage(std::to_string(x) + " " + std::to_string(y) + "\n");
}

void Zappy::Client::PlayerRunState::update(Client& client, std::chrono::nanoseconds dt)
{
    if (!m_Player) {
        client.send("ko\n");
        client.setState(std::make_unique<HandshakeState>(false));
        return;
    }

    if (hasMessages()) {
        std::string events = popMessages();
        client.send(events);
    }
}
