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
    , m_State(std::make_unique<HandshakeState>())
    , m_NextState()
    , m_ClientName(m_Socket)
{

}

Zappy::Client::Client::Client(Client&& other)
    : m_Socket()
    , m_State()
    , m_NextState()
    , m_ClientName()
{
    swap(other);
}

void Zappy::Client::Client::update(std::chrono::nanoseconds dt)
{
    if (m_NextState) {
        m_State.swap(m_NextState);
        m_NextState = nullptr;
    }

    m_State->update(*this, dt);
}

void Zappy::Client::Client::swap(Client& other)
{
    m_Socket.swap(other.m_Socket);
    m_State.swap(other.m_State);
    m_NextState.swap(other.m_NextState);
    m_ClientName.swap(other.m_ClientName);
}
