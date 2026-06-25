/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Client class
*/

#include "Client.hpp"
#include <zappy/client.hpp>
#include <zappy/utils.hpp>


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

void Zappy::Client::Client::update(Game::Game& game, std::chrono::nanoseconds dt)
{
    if (m_NextState) {
        m_State.swap(m_NextState);
        m_NextState = nullptr;
    }

    pushCommand();
    m_State->setGame(game);
    m_State->update(*this, dt);
}

bool Zappy::Client::Client::readline(std::string& output)
{
    output.clear();

    if (m_QueuedCommands.empty())
        return false;

    output.swap(m_QueuedCommands.back());
    output.pop_back();

    return true;
}

void Zappy::Client::Client::swap(Client& other)
{
    m_Socket.swap(other.m_Socket);
    m_State.swap(other.m_State);
    m_NextState.swap(other.m_NextState);
    m_ClientName.swap(other.m_ClientName);
}

void Zappy::Client::Client::pushCommand()
{
    std::string command;

    if (!m_Socket.readUntil(command, '\n'))
        return;

    if (m_QueuedCommands.size() >= COMMAND_LIMIT) {
        Zappy::logger.debug() << m_ClientName << ": discarding command " << Zappy::logger.escape(command) << std::endl;
        return;
    }

    m_QueuedCommands.push_front(std::move(command));
}
