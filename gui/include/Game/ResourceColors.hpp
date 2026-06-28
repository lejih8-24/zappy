/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Shared GUI resource colors
*/

#pragma once

#include "Graphics/Types.hpp"
#include <game.hpp>

#include <array>

namespace GUI {

inline constexpr std::array<Color, Zappy::Game::Resources::RESOURCE_COUNT> ResourceColors = {
    Colors::Brown,     //? Food
    Colors::LightGray, //? Linemate
    Colors::Violet,    //? Deraumere
    Colors::Yellow,    //? Sibur
    Colors::Orange,    //? Mendiane
    Colors::SkyBlue,   //? Phiras
    Colors::Magenta,   //? Thystame
};

}
