/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Built-in primitive theme (cubes and spheres)
*/

#include "Theme/DefaultTheme.hpp"

#include "raylib.h"

#include <array>

static constexpr std::array<Color, 7> RESOURCE_COLORS = {
    BROWN,     // Food
    LIGHTGRAY, // Linemate
    VIOLET,    // Deraumere
    YELLOW,    // Sibur
    ORANGE,    // Mendiane
    SKYBLUE,   // Phiras
    MAGENTA,   // Thystame
};

namespace GUI {

void DefaultTheme::drawTile(Vector3 pos, Vector3 size, bool isLight) const
{
    DrawCubeV(pos, size, isLight ? GREEN : DARKGREEN);
    DrawCubeWiresV(pos, size, BLACK);
}

void DefaultTheme::drawResource(std::size_t resourceIndex, Vector3 pos) const
{
    DrawCubeV(pos, { 0.32F, 0.20F, 0.32F }, RESOURCE_COLORS[resourceIndex]);
}

void DefaultTheme::drawPlayer(Vector3 pos, float rotationDeg, Player::AnimState state) const
{
    (void)rotationDeg;
    (void)state;
    DrawCubeV(pos, { 0.75f, 1.1f, 0.75f }, BLUE);
}

void DefaultTheme::drawEgg(Vector3 pos) const
{
    DrawSphere(pos, 0.35f, RAYWHITE);
}

float DefaultTheme::getPlayerLabelHeight() const
{
    return 1.5f;
}

float DefaultTheme::getPlayerLabelScale() const
{
    return 140.0f;
}

Color DefaultTheme::getBackgroundColor() const
{
    return DARKBLUE;
}

}
