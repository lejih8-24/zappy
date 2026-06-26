/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** HandshakeState class
*/

#include "HandshakeState.hpp"
#include <zappy/client.hpp>


Zappy::Client::HandshakeState::HandshakeState(bool sendWelcome)
    : BaseState()
    , m_TeamName()
    , m_WelcomeSent(!sendWelcome)
{

}

void Zappy::Client::HandshakeState::update(Client& client, std::chrono::nanoseconds)
{
    if (!m_WelcomeSent) {
        auto amnt = client.send(WELCOME_MESSAGE);
        m_WelcomeSent = amnt == WELCOME_MESSAGE.length();
        return;
    }

    if (!client.readline(m_TeamName))
        return;

    if (m_TeamName == "GRAPHIC")
        client.setState(std::make_unique<GuiState>());
    else
        client.setState(std::make_unique<PlayerRunState>(std::move(m_TeamName)));
}
