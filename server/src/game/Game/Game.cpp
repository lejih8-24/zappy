/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Game class
*/

#include "Game.hpp"
#include <zappy/game.hpp>
#include <numbers>
#include <random>
#include <cmath>


using std::chrono_literals::operator""ms;


std::mt19937 Zappy::Game::Game::s_RNG(std::random_device{}());


Zappy::Game::Game::Game() noexcept
    : m_Map(10, 10)
    , m_Players()
    , m_Eggs()
    , m_Teams()
    , m_GraphicsEvents()
    , m_GameSpeed(1)
    , m_EvolveGroups()
{
    regenerateResources();
}

void Zappy::Game::Game::update(std::chrono::nanoseconds dt)
{
    m_GraphicsEvents.clear();
    m_TimeSinceResourceRespawn += dt;

    if (m_TimeSinceResourceRespawn >= RESOURCE_RESPAWN_DELAY / m_GameSpeed) {
        regenerateResources();
        m_TimeSinceResourceRespawn = m_TimeSinceResourceRespawn.zero();
    }

    for (auto it = m_EvolveGroups.begin(); it != m_EvolveGroups.end(); ++it) {
        it->timeLeft -= dt;
        if (it->timeLeft > 0.0ms)
            continue;

        endPlayerIncantation(*it);
        it = m_EvolveGroups.erase(it);
        if (it == m_EvolveGroups.end())
            break;
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

    auto& team = m_Teams[std::string(player.team())];
    team.maxMembers++;

    m_GraphicsEvents.emplace_back(Event::playerFork(player.id()));
    m_GraphicsEvents.emplace_back(Event::eggNew(egg.id(), egg.parentId(), egg.getPosition()));
}

auto Zappy::Game::Game::hatchEgg(std::string_view team) -> Player*
{
    for (auto it = m_Eggs.begin(); it != m_Eggs.end(); ++it) {
        if (it->team() != team)
            continue;

        auto [x, y] = it->getPosition();

        Player& player = emplacePlayer(team);
        player.moveTo(x, y);
        player.setOrientation(static_cast<Orientation>(random(1u, 4u)));
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

std::string Zappy::Game::Game::playerLook(const Player& player)
{
    std::string result = "[";
    auto computeOffset = lookOffsetFnc(player.orientation());

    auto [mapX, mapY] = m_Map.size();
    auto [x, y] = player.position();

    for (unsigned int i = 0; i <= player.level(); i++) {
        unsigned int limit = 2 * i + 1;

        for (unsigned int j = 0; j < limit; j++) {
            auto [offsetX, offsetY] = computeOffset(i, j);
            std::pair<unsigned int, unsigned int> pos = {
                (mapX + offsetX + x) % mapX,
                (mapY + offsetY + y) % mapY,
            };

            // Check for players
            for (const auto& [_, player] : m_Players) {
                if (player.position() != pos)
                    continue;
                result += " player";
                break;
            }

            // Check for resources
            auto& resources = m_Map[pos];
            for (unsigned int i = 0; i < Resources::RESOURCE_COUNT; i++) {
                ResourceType type = static_cast<ResourceType>(i);
                if (resources[type] == 0)
                    continue;
                result.append(" ").append(to_string(type));
            }

            if (i != player.level() || j != limit - 1)
                result += ',';

        }
    }

    result += " ]";
    return result;
}

void Zappy::Game::Game::playerFeed(Player& player)
{
    if (player.inventory().food == 0)
        return;

    player.inventory().food--;
    player.addTimeToLive(FOOD_ALIVE_TIME / m_GameSpeed);

    m_GraphicsEvents.emplace_back(Event::playerInventory(player.id(), player.position(), player.inventory()));
}

bool Zappy::Game::Game::playerCollectResource(Player& player, ResourceType type)
{
    auto& resource = m_Map[player.position()][type];
    if (resource == 0)
        return false;

    resource--;
    player.inventory()[type]++;
    m_GraphicsEvents.emplace_back(Event::playerCollect(player.id(), static_cast<unsigned int>(type)));

    return true;
}

bool Zappy::Game::Game::playerDropResource(Player& player, ResourceType type)
{
    auto& resource = player.inventory()[type];
    if (resource == 0)
        return false;

    resource--;
    m_Map[player.position()][type]++;
    m_GraphicsEvents.emplace_back(Event::playerDrop(player.id(), static_cast<unsigned int>(type)));

    return true;
}

void Zappy::Game::Game::playerBroadcast(const Player& sender, std::string_view msg)
{
    m_GraphicsEvents.emplace_back(Event::playerBroadcast(sender.id(), msg));

    for (auto& [_, player] : m_Players) {
        if (&player == &sender)
            continue;

        auto orientation = orientationTo(player, sender);
        player.addMessage("message " + std::to_string(orientation) + ", " + std::string(msg));
    }
}

auto Zappy::Game::Game::doPlayerIncantation(const Player& initiator) -> EvolutionGroup*
{
    if (initiator.level() >= MAX_PLAYER_LEVEL)
        return nullptr;

    auto& requirements = INCANTATION_REQUIREMENTS[initiator.level()];
    auto& availableResources = m_Map[initiator.position()];

    // The initiator's tile doesn't contain the
    // required resources
    if (!availableResources.contains(requirements.resources))
        return nullptr;

    auto& group = m_EvolveGroups.emplace_back();

    for (auto& [_, player] : m_Players) {
        if (player.position() != initiator.position())
            continue;
        if (player.level() != initiator.level())
            continue;

        group.players.push_back(player.id());
    }

    // Not enough participating players
    if (group.players.size() < requirements.players) {
        m_EvolveGroups.pop_back();
        return nullptr;
    }

    m_GraphicsEvents.emplace_back(Event::playerIncantationStart(initiator.position(), initiator.level(), group.players));

    // Note: don't remove resources from tile;
    // they are checked again at the end of the
    // incantation.

    group.level    = initiator.level();
    group.position = initiator.position();
    group.timeLeft = PLAYER_INCANTATION_COOLDOWN / m_GameSpeed;

    for (auto id : group.players) {
        auto& player = m_Players[id];
        player.addCooldown(group.timeLeft);
    }

    return &group;
}

/**
 * Returns the value from 1 to 8
 * designating the orientation
 * of the player `from` to the
 * orientation to the player `to`
 * according to the assignment.
 */
unsigned int Zappy::Game::Game::orientationTo(const Player& to, const Player& from)
{
    static constexpr unsigned int BUCKETS = 8;

    auto [dist, angle] = shortestDistanceTo(to, from);
    float destinationAngle = std::fmodf(angle + std::numbers::pi, 2.f * std::numbers::pi);

    // Turn range [0; 2pi[ -> [0; 8[ -> [[0; 7]]
    unsigned int angleBucket = (BUCKETS * destinationAngle) / (2.f * std::numbers::pi);

    // Make angle bucket relative to player orientation
    angleBucket = (angleBucket + orientationOffset(to.orientation())) % BUCKETS;

    angleBucket++;  // [[0; 7]] -> [[1; 8]]
    return angleBucket;
}

/**
 * Returns the shortest distance
 * from the player `from` to the
 * player `to`, as well as the
 * angle taken to find the shortest
 * distance.
 */
std::pair<float, float> Zappy::Game::Game::shortestDistanceTo(const Player& to, const Player& from)
{
    auto [mapX, mapY] = m_Map.size();
    auto [toX, toY] = to.position();

    std::array<std::pair<int, int>, 4> possiblePaths = {{
        { toX + mapX, toY },
        { toX - mapX, toY },
        { toX, toY + mapY },
        { toX, toY - mapY },
    }};

    std::pair<float, float> shortestDistance = distanceTo(to.position(), from);
    for (const auto& path : possiblePaths) {
        auto distance = distanceTo(path, from);

        if (distance.first >= shortestDistance.first)
            continue;

        shortestDistance = distance;
    }

    return shortestDistance;
}

/**
 * Finds the distance from the player
 * to the target position, as well as
 * the angle taken to reach it.
 *
 * Note:
 * The angle starts at EAST (positive x),
 * and goes towards NORTH (negative y).
 */
std::pair<float, float> Zappy::Game::Game::distanceTo(std::pair<int, int> position, const Player& player)
{
    auto [posX, posY] = position;
    auto [playerX, playerY] = player.position();

    float dist = std::sqrtf(
        (posX - playerX) * (posX - playerX) +
        (posY - playerY) * (posY - playerY)
    );

    float angleNorm = normalizedAtan2(
        static_cast<float>(posY) - playerY,
        static_cast<float>(posX) - playerX
    );

    return { dist, angleNorm };
}

/**
 * Returns an angle in the
 * range [0; 2pi].
 */
float Zappy::Game::Game::normalizedAtan2(float y, float x)
{
    // Note: we invert y because going up / north
    // on our map means decreasing y (i.e. it's inverted).
    float angle = std::atan2f(-y, x == 0.f ? 0.01f : x);

    return angle > 0.f
        ? angle
        : angle + std::numbers::pi * 2.f;
}

/**
 * Returns the offset that needs to be
 * added to the angle bucket based
 * on the player's orientation. It follows
 * the following diagram:
 *
 *      N                +2
 *
 * W         E      +4        +0
 *
 *      S                +6
 */
unsigned int Zappy::Game::Game::orientationOffset(Orientation orientation)
{
    switch (orientation) {
        case Orientation::EAST:  return 0;
        case Orientation::NORTH: return 2;
        case Orientation::WEST:  return 4;
        case Orientation::SOUTH: return 6;
        default:                 return 0;
    }
}

bool Zappy::Game::Game::isSuccessfulEvolution(const EvolutionGroup& group) const
{
    auto& requirements = INCANTATION_REQUIREMENTS[group.level];
    auto& availableResources = m_Map[group.position];

    // the required resources aren't
    // available anymore
    if (!availableResources.contains(requirements.resources))
        return false;

    std::size_t participants = 0;

    for (auto id : group.players) {
        // player may have died
        if (!m_Players.contains(id))
            continue;

        // player moved / got moved away
        if (m_Players.at(id).position() != group.position)
            continue;

        participants++;
    }

    return participants >= requirements.players;
}

void Zappy::Game::Game::endPlayerIncantation(EvolutionGroup& group)
{
    bool success = isSuccessfulEvolution(group);

    m_GraphicsEvents.emplace_back(Event::playerIncantationEnd(group.position, success));

    for (auto id : group.players) {
        if (!m_Players.contains(id))
            continue;

        auto& player = m_Players.at(id);

        if (success) {
            player.levelUp();
            player.addMessage("Current level: " + std::to_string(player.level()));
        } else {
            player.addMessage("ko");
        }
    }
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
}

void Zappy::Game::Game::regenerateResource(ResourceType type, unsigned int amount)
{
    for (unsigned int i = 0; i < amount; i++) {
        auto tilePos = randomTileResourcePosition();
        m_Map[tilePos][type]++;
        m_GraphicsEvents.emplace_back(Event::tileContents(tilePos.first, tilePos.second, m_Map[tilePos]));
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

/**
 * Returns a function pointer that
 * computes the offset of the look
 * position based on the given orientation.
 *
 * North / Up:
 * i = 0 =>                                  (-i+j;-i)
 * i = 1 =>                       (-1+j;-i), (-i+j;-i), (-i+j;-i)
 * i = 2 =>            (-i+j;-i), (-i+j;-i), (-i+j;-i), (-i+j;-i), (-i+j;-i)
 * i = 3 => (-i+j;-i), (-i+j;-i), (-i+j;-i), (-i+j;-i), (-i+j;-i), (-i+j;-i), (-i+j;-i)
 *
 * South / Down:
 * i = 0 =>                               (i-j;-i)
 * i = 1 =>                     (i-j;-i), (i-j;-i), (i-j;-i)
 * i = 2 =>           (i-j;-i), (i-j;-i), (i-j;-i), (i-j;-i), (i-j;-i)
 * i = 3 => (i-j;-i), (i-j;-i), (i-j;-i), (i-j;-i), (i-j;-i), (i-j;-i), (i-j;-i)
 *
 * East / Right:
 * i = 0 =>                               (i;-i+j)
 * i = 1 =>                     (i;-i+j), (i;-i+j), (i;-i+j)
 * i = 2 =>           (i;-i+j), (i;-i+j), (i;-i+j), (i;-i+j), (i;-i+j)
 * i = 3 => (i;-i+j), (i;-i+j), (i;-i+j), (i;-i+j), (i;-i+j), (i;-i+j), (i;-i+j)
 *
 * West / Left:
 * i = 0 =>                               (-i;i-j)
 * i = 1 =>                     (-i;i-j), (-i;i-j), (-i;i-j)
 * i = 2 =>           (-i;i-j), (-i;i-j), (-i;i-j), (-i;i-j), (-i;i-j)
 * i = 3 => (-i;i-j), (-i;i-j), (-i;i-j), (-i;i-j), (-i;i-j), (-i;i-j), (-i;i-j)
 */
auto Zappy::Game::Game::lookOffsetFnc(Orientation orientation) -> LookOffset
{
    switch (orientation) {
        case Orientation::NORTH: return [](int i, int j) -> std::pair<int, int> { return { -i + j, -i }; };
        case Orientation::SOUTH: return [](int i, int j) -> std::pair<int, int> { return {  i - j,  i }; };
        case Orientation::EAST:  return [](int i, int j) -> std::pair<int, int> { return {  i, -i + j }; };
        case Orientation::WEST:  return [](int i, int j) -> std::pair<int, int> { return { -i,  i - j }; };
        default: return nullptr;
    }
}
