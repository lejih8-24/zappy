/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Shared GUI resource colors
*/

#pragma once

#include <game.hpp>
#include "raylib.h"

#include <array>

namespace GUI {

inline constexpr std::array<Color, Zappy::Game::Resources::RESOURCE_COUNT> ResourceColors = {
    BROWN,     //? Food
    LIGHTGRAY, //? Linemate
    VIOLET,    //? Deraumere
    YELLOW,    //? Sibur
    ORANGE,    //? Mendiane
    SKYBLUE,   //? Phiras
    MAGENTA,   //? Thystame
};

}
