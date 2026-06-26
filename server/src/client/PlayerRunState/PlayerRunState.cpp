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


Zappy::Client::PlayerRunState::PlayerRunState(Game::Player& player)
    : QueueState()
    , m_Player(player)
{

}

void Zappy::Client::PlayerRunState::init()
{

}

void Zappy::Client::PlayerRunState::update(Client& client, std::chrono::nanoseconds dt)
{
    if (hasMessages()) {
        std::string events = popMessages();
        client.send(events);
    }

    std::string command;
    if (!client.readline(command))
        return;

    toLowercase(command);
    runCommand(command);
}

void Zappy::Client::PlayerRunState::toLowercase(std::string& repr)
{
    // See https://en.cppreference.com/cpp/string/byte/tolower
    std::transform(
        repr.begin(),
        repr.end(),
        repr.begin(),
        [](unsigned char c){ return std::tolower(c); }
    );
}

void Zappy::Client::PlayerRunState::runCommand(std::string& command)
{

}
