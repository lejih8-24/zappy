/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Game class
*/

#include "Game.hpp"
#include <zappy/game.hpp>
#include <random>


std::mt19937 Zappy::Game::Game::s_RNG(std::random_device{}());


Zappy::Game::Game::Game() noexcept
    : m_Map(10, 10)
    , m_Players()
    , m_Eggs()
    , m_Teams()
    , m_GraphicsEvents()
    , m_GameSpeed(1)
{
    regenerateResources();
}

void Zappy::Game::Game::update(std::chrono::nanoseconds dt)
{
    m_GraphicsEvents.clear();
    m_TimeSinceResourceRespawn += dt;

    if (m_TimeSinceResourceRespawn >= RESOURCE_RESPAWN_DELAY / m_GameSpeed) {
        regenerateResources();
    }
}

std::size_t Zappy::Game::Game::activePlayers() const
{
    std::size_t total = 0;

    for (const auto& [_, team] : m_Teams) {
        total += team.members;
    }

    return total;
}

void Zappy::Game::Game::setTeams(std::span<std::string> names, std::size_t maxMembers)
{
    m_Teams.clear();

    for (auto& name : names) {
        Team team(name, maxMembers);

        for (std::size_t i = 0; i < maxMembers; i++) {
            auto [randX, randY] = randomPosition();
            m_Eggs.emplace_back(-1, name, randX, randY);
        }

        m_Teams[std::move(name)] = std::move(team);
    }
}

void Zappy::Game::Game::playerLayEgg(const Player& player)
{
    auto [x, y] = player.position();
    auto& egg = m_Eggs.emplace_back(player.id(), player.team(), x, y);
    m_GraphicsEvents.push_back(Event::eggNew(egg.id(), egg.parentId(), egg.getPosition()));
}

auto Zappy::Game::Game::hatchEgg(std::string_view team) -> Player*
{
    for (auto it = m_Eggs.begin(); it != m_Eggs.end(); ++it) {
        if (it->team() != team)
            continue;

        auto [x, y] = it->getPosition();

        Player& player = emplacePlayer(team);
        player.moveTo(x, y);
        m_GraphicsEvents.emplace_back(
            Event::playerNew(
                player.id(), player.position(),
                player.orientation(),
                player.level(), player.team()
        ));

        m_GraphicsEvents.emplace_back(Event::eggHatch(it->id()));
        m_Eggs.erase(it);

        return &player;
    }

    return nullptr;
}

void Zappy::Game::Game::playerUpdatePosition(const Player& player)
{
    m_GraphicsEvents.emplace_back(Event::playerPosition(player.id(), player.position(), player.orientation()));
}

bool Zappy::Game::Game::collectResource(Player& player, ResourceType type)
{
    auto& resource = m_Map[player.position()][type];
    if (resource == 0)
        return false;

    resource--;
    player.inventory()[type]++;
    m_GraphicsEvents.emplace_back(Event::playerCollect(player.id(), static_cast<unsigned int>(type)));

    return true;
}

bool Zappy::Game::Game::doPlayerIncantation(const Player& initiator)
{
    if (initiator.level() >= MAX_PLAYER_LEVEL)
        return false;

    auto& requirements = INCANTATION_REQUIREMENTS[initiator.level()];
    auto& availableResources = m_Map[initiator.position()];

    // The initiator's tile doesn't contain the
    // required resources
    if (!availableResources.contains(requirements.resources))
        return false;

    std::vector<Player*> involvedPlayers;

    for (auto& [_, player] : m_Players) {
        if (player.position() != initiator.position())
            continue;
        if (player.level() != initiator.level())
            continue;

        involvedPlayers.push_back(&player);
    }

    // Not enough participating players
    if (involvedPlayers.size() < requirements.players)
        return false;

    m_GraphicsEvents.emplace_back(Event::playerIncantationStart(initiator.position(), initiator.level(), involvedPlayers));

    // Consume the ritual's resources and level up
    // all involved players
    availableResources -= requirements.resources;
    for (auto player : involvedPlayers)
        player->levelUp();

    return true;
}

void Zappy::Game::Game::killPlayer(const Player& player)
{
    auto& team = playerTeam(player);

    team.members--;
    team.maxMembers--;

    m_GraphicsEvents.emplace_back(Event::playerDie(player.id()));

    auto it = m_Players.find(player.id());
    if (it == m_Players.end())
        return;
    m_Players.erase(it);
}

auto Zappy::Game::Game::emplacePlayer(std::string_view teamName) -> Player&
{
    Player newPlayer(teamName);
    auto& player = m_Players[newPlayer.id()];
    player = std::move(newPlayer);

    Team& team = m_Teams.at(std::string(teamName));
    team.members++;

    return player;
}

void Zappy::Game::Game::regenerateResources()
{
    auto [mapX, mapY] = m_Map.size();
    unsigned int availableSpace = mapX * mapY;

    for (unsigned int type = 0; type < Resources::RESOURCE_COUNT; type++) {
        unsigned int targetResourceAmnt = RESOURCE_DENSITIES[type] * availableSpace;
        unsigned int currentResourceAmount = m_Map.reduce([type](const Resources& current, unsigned int acc) {
            return acc + current[static_cast<ResourceType>(type)];
        }, 0u);

        if (currentResourceAmount >= targetResourceAmnt)
            continue;

        regenerateResource(static_cast<ResourceType>(type), targetResourceAmnt - currentResourceAmount);
    }

    m_TimeSinceResourceRespawn = m_TimeSinceResourceRespawn.zero();
}

void Zappy::Game::Game::regenerateResource(ResourceType type, unsigned int amount)
{
    for (unsigned int i = 0; i < amount; i++) {
        auto tilePos = randomTileResourcePosition();
        m_Map[tilePos][type]++;
    }
}

/**
 * Returns a random tile position,
 * biased towards empty tiles.
 */
std::pair<unsigned int, unsigned int> Zappy::Game::Game::randomTileResourcePosition() const
{
    auto randPos = randomPosition();

    if (m_Map[randPos].empty())
        return randPos;

    auto [mapX, mapY] = m_Map.size();

    for (std::size_t y = 0; y < mapY; y++) {
        for (std::size_t x = 0; x < mapX; x++) {
            if (m_Map[{ x, y }].empty())
                return { x, y };
        }
    }

    return randPos;
}
