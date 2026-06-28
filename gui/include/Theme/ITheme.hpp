/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Abstract theme interface for the pack system
*/

#pragma once

#include "Game/Player.hpp"
#include "Graphics/ICanvas.hpp"
#include <cstddef>

namespace GUI {

class ITheme {
    public:
        virtual ~ITheme() = default;

        virtual void drawTile(const ICanvas &canvas, Vec3 pos, Vec3 size, bool isLight) const = 0;
        virtual void drawResource(const ICanvas &canvas, std::size_t resourceIndex, Vec3 pos) const = 0;
        virtual void drawPlayer(const ICanvas &canvas, Vec3 pos, float rotationDeg, Player::AnimState state,
            float animationElapsed = 0.0F) const = 0;
        virtual void drawEgg(const ICanvas &canvas, Vec3 pos) const = 0;

        virtual float getPlayerLabelHeight() const = 0;
        virtual float getPlayerLabelScale() const = 0;
        virtual float getPlayerAnimationDuration(Player::AnimState state) const = 0;
        virtual Color getBackgroundColor() const = 0;
};

}
