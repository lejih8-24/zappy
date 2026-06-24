/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** HandshakeState class
*/

#include "HandshakeState.hpp"
#include <zappy/client.hpp>


Zappy::Client::HandshakeState::HandshakeState()
    : m_TeamName()
    , m_WelcomeSent(false)
{

}

void Zappy::Client::HandshakeState::update(Client& client, std::chrono::nanoseconds)
{
    if (!m_WelcomeSent) {
        auto amnt = client.socket().write(WELCOME_MESSAGE);
        m_WelcomeSent = amnt == WELCOME_MESSAGE.length();
        return;
    }

    if (!client.socket().readUntil(m_TeamName, '\n'))
        return;

    if (m_TeamName == "GRAPHIC")
        client.setState(std::make_unique<GuiState>());
    else
        client.setState(std::make_unique<PlayerRunState>());
}
