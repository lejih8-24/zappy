/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Event class
*/

#include "Event.hpp"
#include <zappy/game.hpp>


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

std::string Zappy::Game::Event::eggNew(int eggId, int playerId, std::pair<unsigned int, unsigned int> pos)
{
    return
        "enw "
        "#" + std::to_string(eggId) + " "
        "#" + std::to_string(playerId) + " " +
        std::to_string(pos.first) + " " +
        std::to_string(pos.second) +
        "\n";
}
