/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** GuiState class
*/

#include "GuiState.hpp"
#include <charconv>
#include <zappy/client.hpp>
#include <zappy/game.hpp>


const std::unordered_map<std::string_view, Zappy::Client::GuiState::CommandHandler> Zappy::Client::GuiState::COMMAND_HANDLERS = {
    { "msz", mapSizeCommand },
    { "bct", tileContentsCommand },
    { "mct", mapContentsCommand },
    { "tna", teamNamesCommand },
    { "ppo", playerPositionCommand },
    { "plv", playerLevelCommand },
    { "pin", playerInventoryCommand },
    { "sgt", serverGetTimeCommand },
    { "sst", serverSetTimeCommand },
};


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

    for (const auto& [_, player] : game().players()) {
        queueMessage(Game::Event::playerNew(player.id(), player.position(), player.orientation(), player.level(), player.team()));
        queueMessage(Game::Event::playerInventory(player.id(), player.position(), player.inventory()));
    }

    for (const auto& egg : game().eggs()) {
        queueMessage(Game::Event::eggNew(egg.id(), egg.parentId(), egg.getPosition()));
    }
}

void Zappy::Client::GuiState::postUpdate(Client& client)
{
    for (const auto& event : game().graphicsEvents())
        queueMessage(event);

    sendQueuedMessages(client);

    std::string command;
    if (client.readline(command))
        handleCommand(command, client);

    if (hasMessages())
        client.refresh();
}

void Zappy::Client::GuiState::handleCommand(std::string_view command, Client& client)
{
    auto commandEnd = command.find(' ');
    std::string_view args = commandEnd + 1 >= command.size() ? std::string_view() : command.substr(commandEnd + 1);
    command = command.substr(0, commandEnd);

    try {
        auto handler = COMMAND_HANDLERS.at(command);
        handler(args, *this, client);
    } catch (std::out_of_range) {
        logger.warning() << std::string(client) << " (GUI): unknown command " << std::quoted(command) << std::endl;
        queueMessage("suc\n");
        return;
    }
}

void Zappy::Client::GuiState::mapSizeCommand(std::string_view, GuiState& state, Client&)
{
    state.game().mapSize();
    state.queueMessage(Game::Event::mapSize(state.game().mapSize()));
}

void Zappy::Client::GuiState::tileContentsCommand(std::string_view args, GuiState& state, Client& client)
{
    auto [mapX, mapY] = state.game().mapSize();
    auto pos = parsePosition(args);

    if (!pos || pos->first >= mapX || pos->second >= mapY) {
        logger.warning() << std::string(client) << " (GUI): bad bct parameters: " << logger.escape(args, '\"') << std::endl;
        state.queueMessage("sbp\n");
        return;
    }

    const auto& resources = state.game().tiles().at(*pos);
    state.queueMessage(Game::Event::tileContents(pos->first, pos->second, resources));
}

void Zappy::Client::GuiState::mapContentsCommand(std::string_view, GuiState& state, Client&)
{
    auto [mapX, mapY] = state.game().mapSize();

    for (unsigned int y = 0; y < mapY; y++) {
        for (unsigned int x = 0; x < mapX; x++) {
            const auto& resources = state.game().tiles().at(x, y);
            state.queueMessage(Game::Event::tileContents(x, y, resources));
        }
    }
}

void Zappy::Client::GuiState::teamNamesCommand(std::string_view, GuiState& state, Client&)
{
    for (const auto& [teamName, _] : state.game().teams()) {
        state.queueMessage(Game::Event::teamName(teamName));
    }
}

void Zappy::Client::GuiState::playerPositionCommand(std::string_view args, GuiState& state, Client& client)
{
    // TODO: implement
}

void Zappy::Client::GuiState::playerLevelCommand(std::string_view args, GuiState& state, Client& client)
{
    // TODO: implement
}

void Zappy::Client::GuiState::playerInventoryCommand(std::string_view args, GuiState& state, Client& client)
{
    // TODO: implement
}

void Zappy::Client::GuiState::serverGetTimeCommand(std::string_view args, GuiState& state, Client& client)
{
    // TODO: implement
}

void Zappy::Client::GuiState::serverSetTimeCommand(std::string_view args, GuiState& state, Client& client)
{
    // TODO: implement
}

std::optional<std::pair<unsigned int, unsigned int>> Zappy::Client::GuiState::parsePosition(std::string_view& str)
{
    auto wordEnd = str.find(' ');
    if (wordEnd + 1 >= str.length())
        return std::nullopt;

    std::string_view xRepr = str.substr(0, wordEnd);
    std::string_view yRepr = str.substr(wordEnd + 1);

    std::optional<std::pair<unsigned int, unsigned int>> result = {{ 0, 0 }};

    auto [ptr1, ec1] = std::from_chars(xRepr.begin(), xRepr.end(), result->first);
    auto [ptr2, ec2] = std::from_chars(yRepr.begin(), yRepr.end(), result->second);

    if (ec1 != std::errc() || ec2 != std::errc())
        return std::nullopt;

    return result;
}
