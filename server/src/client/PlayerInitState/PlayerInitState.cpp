/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** PlayerInitState class
*/

#include "PlayerInitState.hpp"
#include <zappy/client.hpp>
#include <zappy/game.hpp>


Zappy::Client::PlayerInitState::PlayerInitState(const std::string& teamName)
    : QueueState()
    , m_TeamName(teamName)
    , m_Player(nullptr)
{

}

Zappy::Client::PlayerInitState::PlayerInitState(std::string&& teamName)
    : QueueState()
    , m_TeamName(std::move(teamName))
    , m_Player(nullptr)
{

}

void Zappy::Client::PlayerInitState::init()
{
    m_Player = game().hatchEgg(m_TeamName);
    if (!m_Player)
        return;

    auto& team = game().teams().at(m_TeamName);
    auto [x, y] = game().mapSize();

    queueMessage(std::to_string(team.maxMembers - team.members) + "\n");
    queueMessage(std::to_string(x) + " " + std::to_string(y) + "\n");
}

void Zappy::Client::PlayerInitState::update(Client& client, std::chrono::nanoseconds dt)
{
    if (!m_Player) {
        client.send("ko\n");
        client.setState(std::make_unique<HandshakeState>(false));
        return;
    }

    sendQueuedMessages(client);
    client.setState(std::make_unique<PlayerRunState>(*m_Player));
}
