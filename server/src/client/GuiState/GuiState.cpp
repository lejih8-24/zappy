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
    for (const auto& event : game().graphicsEvents())
        m_QueuedEvents.emplace_back(event);

    if (!m_QueuedEvents.empty()) {
        std::string events = mergeEvents();
        client.send(events);
    }

    std::string command;
    if (client.readline(command))
        handleCommand(command);
}

/**
 * Merges all* events into a single
 * string so it can be sent off to
 * the client in a single write() call
 * (*up to a limit of EVENT_MERGE_LIMIT).
 *
 * Assumes m_QueuedEvents.size() > 0.
 */
std::string Zappy::Client::GuiState::mergeEvents()
{
    std::string events = std::move(m_QueuedEvents.front());
    m_QueuedEvents.pop_front();

    for (std::size_t i = 0; !m_QueuedEvents.empty() && i < EVENT_MERGE_LIMIT; i++) {
        events += m_QueuedEvents.front();
        m_QueuedEvents.pop_front();
    }

    return events;
}

void Zappy::Client::GuiState::handleCommand(std::string_view command)
{
    // TODO: implement
}
