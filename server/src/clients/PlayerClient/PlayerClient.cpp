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
    , m_ClientState(std::make_unique<HandshakeState>(*this))
{

}

void Zappy::PlayerClient::update(Tick elapsedTicks)
{
    m_ClientState->update(elapsedTicks);
}

void Zappy::PlayerClient::HandshakeState::update(Tick elapsedTicks)
{

}
