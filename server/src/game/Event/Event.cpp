/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Event class
*/

#include "Event.hpp"


std::string Zappy::Game::Event::mapSize(unsigned int width, unsigned int height)
{
    return "msz " + std::to_string(width) + " " + std::to_string(height) + "\n";
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
