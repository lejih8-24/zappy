/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** PlayerClient class
*/

#include "PlayerClient.hpp"


Zappy::PlayerClient::PlayerClient(Lattice::Socket&& socket)
    : BaseClient(std::move(socket))
    , m_PlayerData()
{

}

void Zappy::PlayerClient::update(Tick elapsedTicks)
{
    if (!m_PlayerData) {
        completeHandshake();
        return;
    }
}

void Zappy::PlayerClient::completeHandshake()
{

}
