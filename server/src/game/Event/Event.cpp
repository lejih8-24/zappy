/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Event class
*/

#include "Event.hpp"
#include <zappy/game.hpp>


using std::string_literals::operator ""s;


std::string Zappy::Game::Event::mapSize(unsigned int width, unsigned int height)
{
    return "msz " + std::to_string(width) + " " + std::to_string(height) + "\n";
}

std::string Zappy::Game::Event::tileContents(unsigned int x, unsigned int y, const Resources& contents)
{
    return "bct " + std::to_string(x)                  + " "
                  + std::to_string(y)                  + " "
                  + std::to_string(contents.food)      + " "
                  + std::to_string(contents.linemate)  + " "
                  + std::to_string(contents.deraumere) + " "
                  + std::to_string(contents.sibur)     + " "
                  + std::to_string(contents.mendiane)  + " "
                  + std::to_string(contents.phiras)    + " "
                  + std::to_string(contents.thystame)  + "\n";
}

std::string Zappy::Game::Event::teamName(std::string_view name)
{
    return "tna " + std::string(name) + "\n";
}

std::string Zappy::Game::Event::playerNew(int playerId, std::pair<unsigned int, unsigned int> pos, Orientation orientation, int level, std::string_view team)
{
    unsigned int orient = static_cast<unsigned int>(orientation);

    return "pnw #" + std::to_string(playerId)   + " "
                   + std::to_string(pos.first)  + " "
                   + std::to_string(pos.second) + " "
                   + std::to_string(orient)     + " "
                   + std::to_string(level)      + " "
                   + std::string(team)          + "\n";
}

std::string Zappy::Game::Event::playerPosition(int playerId, std::pair<unsigned int, unsigned int> pos, Orientation orientation)
{
    return "ppo #" + std::to_string(playerId) + " "
                   + std::to_string(pos.first) + " "
                   + std::to_string(pos.second) + " "
                   + std::to_string(static_cast<unsigned int>(orientation)) + "\n";
}

std::string Zappy::Game::Event::playerLevel(int playerId, unsigned int level)
{
    return "plv #" + std::to_string(playerId) + " " + std::to_string(level) + "\n";
}

std::string Zappy::Game::Event::playerInventory(int playerId, std::pair<unsigned int, unsigned int> pos, const Resources& inventory)
{
    return "pin #" + std::to_string(playerId)            + " "
                   + std::to_string(pos.first)           + " "
                   + std::to_string(pos.second)          + " "
                   + std::to_string(inventory.food)      + " "
                   + std::to_string(inventory.linemate)  + " "
                   + std::to_string(inventory.deraumere) + " "
                   + std::to_string(inventory.sibur)     + " "
                   + std::to_string(inventory.mendiane)  + " "
                   + std::to_string(inventory.phiras)    + " "
                   + std::to_string(inventory.thystame)  + "\n";
}

std::string Zappy::Game::Event::playerEject(int playerId)
{
    return "pex #" + std::to_string(playerId) + "\n";
}

std::string Zappy::Game::Event::playerBroadcast(int playerId, std::string_view message)
{
    std::string evt = "pbc #" + std::to_string(playerId) + " " + std::string(message);

    if (!evt.ends_with('\n'))
        evt.push_back('\n');

    return evt;
}

std::string Zappy::Game::Event::playerDie(int playerId)
{
    return "pdi #" + std::to_string(playerId) + "\n";
}

std::string Zappy::Game::Event::playerCollect(int playerId, unsigned int resource)
{
    return "pgt #" + std::to_string(playerId) + " " + std::to_string(resource) + "\n";
}

std::string Zappy::Game::Event::playerDrop(int playerId, unsigned int resource)
{
    return "pdr #" + std::to_string(playerId) + " " + std::to_string(resource) + "\n";
}

std::string Zappy::Game::Event::playerIncantationStart(std::pair<unsigned int, unsigned int> pos, unsigned int level, std::span<int> players)
{
    std::string ids;

    for (auto id : players) {
        ids += std::to_string(id) + " ";
    }

    return playerIncantationStart(pos, level, ids);
}

std::string Zappy::Game::Event::playerIncantationStart(std::pair<unsigned int, unsigned int> pos, unsigned int level, std::string_view playerIds)
{
    return "pic " + std::to_string(pos.first)  + " "
                  + std::to_string(pos.second) + " "
                  + std::to_string(level)      + " "
                  + std::string(playerIds)     + "\n";
}

std::string Zappy::Game::Event::playerIncantationEnd(std::pair<unsigned int, unsigned int> pos, bool success)
{
    return "pie " + std::to_string(pos.first)                 + " "
                  + std::to_string(pos.second)                + " "
                  + std::to_string(static_cast<int>(success)) + "\n";
}

std::string Zappy::Game::Event::playerFork(int playerId)
{
  return "pfk #" + std::to_string(playerId) + "\n";
}

std::string Zappy::Game::Event::eggNew(int eggId, int playerId, std::pair<unsigned int, unsigned int> pos)
{
    return "enw "s + "#" + std::to_string(eggId)    + " "
                   + "#" + std::to_string(playerId) + " "
                   + std::to_string(pos.first)      + " "
                   + std::to_string(pos.second)     + "\n";
}

std::string Zappy::Game::Event::eggHatch(int eggId)
{
    return "ebo #" + std::to_string(eggId) + "\n";
}

std::string Zappy::Game::Event::serverGetTime(unsigned int speed)
{
  return "sgt " + std::to_string(speed) + "\n";
}

std::string Zappy::Game::Event::serverSetTime(unsigned int speed)
{
  return "sst " + std::to_string(speed) + "\n";
}
