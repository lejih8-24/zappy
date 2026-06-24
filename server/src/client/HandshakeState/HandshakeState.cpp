/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** HandshakeState class
*/

#include "HandshakeState.hpp"
#include <zappy/client.hpp>


Zappy::Client::HandshakeState::HandshakeState(Client& client)
    : m_Client(client)
    , m_TeamName()
    , m_WelcomeSent(false)
{

}

void Zappy::Client::HandshakeState::update(Tick)
{
    if (!m_WelcomeSent) {
        auto amnt = m_Client.socket().write(WELCOME_MESSAGE);
        m_WelcomeSent = amnt == WELCOME_MESSAGE.length();
        return;
    }

    if (!m_Client.socket().readUntil(m_TeamName, '\n'))
        return;

    if (m_TeamName == "GRAPHIC")
        m_Client.setState(std::make_unique<GuiState>(m_Client));
    else
        m_Client.setState(std::make_unique<>());
}
