/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Apply networking events to the GUI game state
*/

#include "Game/GameStateEventHandler.hpp"

#include <algorithm>
#include <utility>

// retirer enum d'orientation pcq raylib gère déjà les degrés d'orientation?
static GUI::Orientation toOrientation(float rotation)
{
    if (rotation == 90.0F)
        return GUI::Orientation::West;
    if (rotation == 180.0F)
        return GUI::Orientation::South;
    if (rotation == 270.0F)
        return GUI::Orientation::East;
    return GUI::Orientation::North;
}

static GUI::Tile &getTile(GUI::GameState &state, int x, int y)
{
    auto tile = std::find_if(state.tiles.begin(), state.tiles.end(), [x, y](const GUI::Tile &value) {
        return value.x == x && value.y == y;
    });

    // If not found in the vector, add it before returning the tile.
    if (tile == state.tiles.end()) {
        state.tiles.emplace_back(x, y);
        return state.tiles.back();
    }
    return *tile;
}

namespace GUI {

GameStateEventHandler::GameStateEventHandler(GameState &state)
    : _state(state)
{
}

void GameStateEventHandler::operator()(const Zappy::Networking::MapSize &event)
{
    _state.mapWidth = event.x;
    _state.mapHeight = event.y;
}

void GameStateEventHandler::operator()(const Zappy::Networking::TileContents &event)
{
    getTile(_state, event.x, event.y).resources = event.resources;
}

void GameStateEventHandler::operator()(const Zappy::Networking::TeamName &event)
{
    // Add only if the team isn't already registered in the vector
    if (std::find(_state.teams.begin(), _state.teams.end(), event.name) == _state.teams.end())
        _state.teams.push_back(event.name);
}

void GameStateEventHandler::operator()(const Zappy::Networking::NewPlayerConnect &event)
{
    Player player(event.id, event.x, event.y, event.level, event.teamName);

    player.orientation = toOrientation(event.rotationDeg);
    _state.players.insert_or_assign(event.id, std::move(player));
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerPosition &event)
{
    auto player = _state.players.find(event.id);

    if (player == _state.players.end())
        return;
    player->second.x = event.x;
    player->second.y = event.y;
    player->second.orientation = toOrientation(event.rotationDeg);
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerLevel &event)
{
    auto player = _state.players.find(event.id);

    if (player != _state.players.end())
        player->second.level = event.level;
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerInventory &event)
{
    auto player = _state.players.find(event.id);

    if (player == _state.players.end())
        return;
    player->second.x = event.x;
    player->second.y = event.y;
    player->second.inventory = event.inventory;
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerCollectResource &event)
{
    auto player = _state.players.find(event.playerId);

    if (player == _state.players.end())
        return;
    ++player->second.inventory[event.resource];
    auto &quantity = getTile(_state, player->second.x, player->second.y).resources[event.resource];
    if (quantity > 0)
        --quantity;
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerDropResource &event)
{
    auto player = _state.players.find(event.playerId);

    if (player == _state.players.end())
        return;
    auto &quantity = player->second.inventory[event.resource];
    if (quantity > 0)
        --quantity;
    ++getTile(_state, player->second.x, player->second.y).resources[event.resource];
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerDie &event)
{
    _state.players.erase(event.playerId);
}

void GameStateEventHandler::operator()(const Zappy::Networking::EggCreate &event)
{
    _state.eggs.insert_or_assign(event.eggId, Egg(event.eggId, event.playerId, event.x, event.y));
}

void GameStateEventHandler::operator()(const Zappy::Networking::EggConnect &event)
{
    _state.eggs.erase(event.eggId);
}

void GameStateEventHandler::operator()(const Zappy::Networking::EggDie &event)
{
    _state.eggs.erase(event.eggId);
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerIncantationStart &event)
{
    Incantation incantation(_state.incantations.size(), event.x, event.y, event.level, event.playerIds);

    incantation.active = true;
    _state.incantations.push_back(std::move(incantation));
    for (int id : event.playerIds) {
        auto player = _state.players.find(id);
        if (player != _state.players.end())
            player->second.isIncantating = true;
    }
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerIncantationEnd &event)
{
    // "pie" event has no incantation ID, so get the latest active one at these coordinates
    auto incantation = std::find_if(_state.incantations.rbegin(), _state.incantations.rend(),
        [&event](const Incantation &value) {
            return value.active && value.x == static_cast<int>(event.x)
                && value.y == static_cast<int>(event.y);
        });

    if (incantation == _state.incantations.rend())
        return;
    incantation->active = false;
    // The protocol uses 1 for success and 0 for failure
    incantation->success = event.result == "1";
    // All participants are now free to start another incantation
    for (int id : incantation->playerIds) {
        auto player = _state.players.find(id);
        if (player != _state.players.end())
            player->second.isIncantating = false;
    }
}

void GameStateEventHandler::operator()(const Zappy::Networking::ServerGetTime &event)
{
    _state.timeUnit = event.time;
}

void GameStateEventHandler::operator()(const Zappy::Networking::ServerSetTime &event)
{
    _state.timeUnit = event.time;
}

void GameStateEventHandler::operator()(const Zappy::Networking::ServerGameEnd &event)
{
    _state.gameEnded = true;
    _state.winningTeam = event.winnerTeam;
}

}
