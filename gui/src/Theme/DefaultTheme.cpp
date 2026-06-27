/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Built-in primitive theme (cubes and spheres)
*/

#include "Theme/DefaultTheme.hpp"

#include <array>

static constexpr std::array<GUI::Color, 7> RESOURCE_COLORS = {
    GUI::Colors::Brown,     // Food
    GUI::Colors::LightGray, // Linemate
    GUI::Colors::Violet,    // Deraumere
    GUI::Colors::Yellow,    // Sibur
    GUI::Colors::Orange,    // Mendiane
    GUI::Colors::SkyBlue,   // Phiras
    GUI::Colors::Magenta,   // Thystame
};

namespace GUI {

void DefaultTheme::drawTile(const Canvas &canvas, Vec3 pos, Vec3 size, bool isLight) const
{
    canvas.drawCube(pos, size, isLight ? Colors::Green : Colors::DarkGreen);
    canvas.drawCubeWires(pos, size, Colors::Black);
}

void DefaultTheme::drawResource(const Canvas &canvas, std::size_t resourceIndex, Vec3 pos) const
{
    canvas.drawCube(pos, {0.32F, 0.20F, 0.32F}, RESOURCE_COLORS[resourceIndex]);
}

void DefaultTheme::drawPlayer(const Canvas &canvas, Vec3 pos, float rotationDeg, Player::AnimState state,
    float animationElapsed) const
{
    (void)rotationDeg;
    (void)state;
    (void)animationElapsed;
    canvas.drawCube(pos, {0.75f, 1.1f, 0.75f}, Colors::Blue);
}

void DefaultTheme::drawEgg(const Canvas &canvas, Vec3 pos) const
{
    canvas.drawSphere(pos, 0.35f, Colors::RayWhite);
}

float DefaultTheme::getPlayerLabelHeight() const
{
    return 1.5f;
}

float DefaultTheme::getPlayerLabelScale() const
{
    return 140.0f;
}

float DefaultTheme::getPlayerAnimationDuration(Player::AnimState state) const
{
    (void)state;
    return 1.0F;
}

Color DefaultTheme::getBackgroundColor() const
{
    return Colors::DarkBlue;
}

}
