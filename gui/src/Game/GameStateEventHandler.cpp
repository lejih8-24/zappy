/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Apply networking events to the GUI game state
*/

#include "Game/GameStateEventHandler.hpp"

#include <algorithm>
#include <utility>
#include "raylib.h"

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

static float getMovementDuration(const GUI::GameState &state)
{
    if (state.timeUnit <= 0)
        return 0.35F;
    return std::max(7.0F / static_cast<float>(state.timeUnit), 0.20F);
}

static float getClosestWrappedTarget(float current, int target, std::size_t mapSize)
{
    if (mapSize == 0)
        return static_cast<float>(target);

    // Zappy maps wrap around: choose the closest visual target so 0 -> width - 1
    // moves through the border instead of crossing the whole map
    float mapSizeF = static_cast<float>(mapSize);
    float wrappedTarget = static_cast<float>(target);
    float delta = wrappedTarget - current;

    if (delta > mapSizeF / 2.0F)
        wrappedTarget -= mapSizeF;
    else if (delta < -mapSizeF / 2.0F)
        wrappedTarget += mapSizeF;
    return wrappedTarget;
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

    player.rotationDeg = event.rotationDeg;
    _state.players.insert_or_assign(event.id, std::move(player));
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerPosition &event)
{
    auto player = _state.players.find(event.id);

    if (player == _state.players.end())
        return;
    bool hasMoved = player->second.x != event.x || player->second.y != event.y;
    float now = GetTime();

    if (hasMoved) {
        // Start from the currently displayed position, not the old tile, to avoid a jump
        // when a new ppo arrives while the previous interpolation is still running
        Player::DisplayPosition start = player->second.getDisplayPosition(now);
        float duration = getMovementDuration(_state);
        Player::DisplayPosition target = {
            getClosestWrappedTarget(start.x, event.x, _state.mapWidth),
            getClosestWrappedTarget(start.y, event.y, _state.mapHeight),
        };

        player->second.startMovement(now, start, target, duration);
        // Special states like Dead/Incantation/Broadcast keep priority over Walk
        Player::AnimState currentState = player->second.getEffectiveAnimState(now);
        if (currentState == Player::AnimState::Idle || currentState == Player::AnimState::Walk)
            player->second.setAnimState(Player::AnimState::Walk, now, duration);
    }
    player->second.x = event.x;
    player->second.y = event.y;
    player->second.rotationDeg = event.rotationDeg;
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerLevel &event)
{
    auto player = _state.players.find(event.id);

    if (player == _state.players.end())
        return;
    player->second.level = event.level;
    player->second.setAnimState(Player::AnimState::LevelUp, GetTime(), 2.5F);
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerInventory &event)
{
    auto player = _state.players.find(event.id);

    if (player == _state.players.end())
        return;
    // pin (player inventory) is a state sync, not a movement event: snap if the server position differs
    if (player->second.x != event.x || player->second.y != event.y) {
        player->second.x = event.x;
        player->second.y = event.y;
        player->second.snapDisplayPosition(event.x, event.y);
    }
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
    auto player = _state.players.find(event.playerId);

    if (player == _state.players.end())
        return;
    player->second.alive = false;
    player->second.setAnimState(Player::AnimState::Dead, GetTime());
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
        if (player != _state.players.end()) {
            player->second.setAnimState(Player::AnimState::Incantation, GetTime());
        }
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
            player->second.setAnimState(Player::AnimState::Idle, GetTime());
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

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerBroadcast &event)
{
    auto player = _state.players.find(event.id);

    if (player == _state.players.end())
        return;
    player->second.setAnimState(Player::AnimState::Broadcast, GetTime(), 2.0F);
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerExpulsion &event)
{
    auto player = _state.players.find(event.id);

    if (player == _state.players.end())
        return;
    player->second.setAnimState(Player::AnimState::Eject, GetTime(), 1.0F);
}

void GameStateEventHandler::operator()(const Zappy::Networking::PlayerLayEgg &event)
{
    auto player = _state.players.find(event.playerId);

    if (player == _state.players.end())
        return;
    player->second.setAnimState(Player::AnimState::LayingEgg, GetTime(), 1.5F);
}

}
