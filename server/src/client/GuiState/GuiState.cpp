/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** GuiState class
*/

#include "GuiState.hpp"
#include <zappy/client.hpp>


Zappy::Client::GuiState::GuiState()
    : BaseState()
    , m_QueuedEvents()
{

}

void Zappy::Client::GuiState::init()
{
    auto [mapX, mapY] = game().mapSize();

    m_QueuedEvents.emplace_back(Game::Event::mapSize(mapX, mapY));
    m_QueuedEvents.emplace_back(Game::Event::serverGameSpeed(game().gameSpeed()));

    for (std::size_t y = 0; y < mapY; y++) {
        for (std::size_t x = 0; x < mapX; x++) {
            const auto& tile = game().tiles()[{ x, y }];
            m_QueuedEvents.emplace_back(Game::Event::tileContents(x, y, tile));
        }
    }

    for (const auto& [teamName, _] : game().teams()) {
        m_QueuedEvents.emplace_back(Game::Event::teamName(teamName));
    }

    for (const auto& egg : game().eggs()) {
        m_QueuedEvents.emplace_back(Game::Event::eggNew(egg.id(), egg.parentId(), egg.getPosition()));
    }
}

void Zappy::Client::GuiState::update(Client& client, std::chrono::nanoseconds dt)
{
    // only one event can be sent per update,
    // so add all new server events to the queue
    for (const auto& event : game().graphicsEvents()) {
        m_QueuedEvents.emplace_back(event);
    }

    if (!m_QueuedEvents.empty()) {
        client.send(m_QueuedEvents.front());
        m_QueuedEvents.pop_front();
    }

    std::string command;
    if (client.readline(command))
        handleCommand(command);
}

void Zappy::Client::GuiState::handleCommand(std::string_view command)
{
    // TODO: implement
}
