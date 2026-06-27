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
    : QueueState()
{

}

void Zappy::Client::GuiState::init()
{
    auto [mapX, mapY] = game().mapSize();

    queueMessage(Game::Event::mapSize(mapX, mapY));
    queueMessage(Game::Event::serverGameSpeed(game().gameSpeed()));

    for (std::size_t y = 0; y < mapY; y++) {
        for (std::size_t x = 0; x < mapX; x++) {
            const auto& tile = game().tiles()[{ x, y }];
            queueMessage(Game::Event::tileContents(x, y, tile));
        }
    }

    for (const auto& [teamName, _] : game().teams()) {
        queueMessage(Game::Event::teamName(teamName));
    }

    for (const auto& egg : game().eggs()) {
        queueMessage(Game::Event::eggNew(egg.id(), egg.parentId(), egg.getPosition()));
    }
}

void Zappy::Client::GuiState::update(Client& client, std::chrono::nanoseconds dt)
{
    for (const auto& event : game().graphicsEvents())
        queueMessage(event);

    sendQueuedMessages(client);

    std::string command;
    if (client.readline(command))
        handleCommand(command);
}

void Zappy::Client::GuiState::handleCommand(std::string_view command)
{
    // TODO: implement
}
