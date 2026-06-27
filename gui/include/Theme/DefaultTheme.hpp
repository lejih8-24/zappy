/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Built-in primitive theme (cubes and spheres)
*/

#pragma once

#include "Theme/ITheme.hpp"

namespace GUI {

class DefaultTheme : public ITheme {
    public:
        void drawTile(const Canvas &canvas, Vec3 pos, Vec3 size, bool isLight) const override;
        void drawResource(const Canvas &canvas, std::size_t resourceIndex, Vec3 pos) const override;
        void drawPlayer(const Canvas &canvas, Vec3 pos, float rotationDeg, Player::AnimState state,
            float animationElapsed = 0.0F) const override;
        void drawEgg(const Canvas &canvas, Vec3 pos) const override;

        float getPlayerLabelHeight() const override;
        float getPlayerLabelScale() const override;
        float getPlayerAnimationDuration(Player::AnimState state) const override;
        Color getBackgroundColor() const override;
};

}
