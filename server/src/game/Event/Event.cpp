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
