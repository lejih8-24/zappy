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


using std::chrono_literals::operator ""ms;


static constexpr std::chrono::duration<double, std::milli> RESOURCE_RESPAWN_DELAY = 20'000ms;

/**
 * Trantor resource densities.
 */
static constexpr std::array<float, Zappy::Game::Resources::RESOURCE_COUNT> RESOURCE_DENSITIES = {{
    /* [FOOD]      = */ 0.50f,
    /* [LINEMATE]  = */ 0.30f,
    /* [DERAUMERE] = */ 0.15f,
    /* [SIBUR]     = */ 0.10f,
    /* [MENDIANE]  = */ 0.10f,
    /* [PHIRAS]    = */ 0.08f,
    /* [THYSTAME]  = */ 0.05f,
}};


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

        auto& player = m_Players.emplace_back(team);
        player.moveTo(x, y);

        m_GraphicsEvents.emplace_back(
            Event::playerNew(
                player.id(), player.position(),
                static_cast<int>(player.orientation()),
                player.level(), player.team()
        ));
        m_GraphicsEvents.emplace_back(Event::eggHatch(it->id()));

        m_Eggs.erase(it);
        return &player;
    }

    return nullptr;
}

void Zappy::Game::Game::killPlayer(const Player& player)
{
    auto& team = playerTeam(player);

    team.members--;
    team.maxMembers--;

    m_GraphicsEvents.emplace_back(Event::playerDie(player.id()));
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
