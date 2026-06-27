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
        void drawTile(Vector3 pos, Vector3 size, bool isLight) const override;
        void drawResource(std::size_t resourceIndex, Vector3 pos, float height) const override;
        void drawPlayer(Vector3 pos, float rotationDeg, Player::AnimState state) const override;
        void drawEgg(Vector3 pos) const override;

        float getPlayerLabelHeight() const override;
        float getPlayerLabelScale() const override;
        Color getBackgroundColor() const override;
};

}
