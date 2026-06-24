/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Client class
*/

#include "Client.hpp"
#include <zappy/client.hpp>


Zappy::Client::Client::Client(Lattice::Socket&& socket)
    : m_Socket(std::move(socket))
    , m_State(std::make_unique<HandshakeState>(*this))
    , m_NextState()
{

}

void Zappy::Client::Client::update(std::chrono::nanoseconds dt)
{
    if (m_NextState) {
        m_State.swap(m_NextState);
        m_NextState = nullptr;
    }

    m_State->update(dt);
}
