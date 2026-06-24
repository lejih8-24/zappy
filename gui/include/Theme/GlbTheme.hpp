/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GLB model theme - renders players via CharacterModel
*/

#pragma once

#include "Theme/ITheme.hpp"
#include "Theme/DefaultTheme.hpp"
#include "Game/CharacterModel.hpp"

namespace GUI {

class GlbTheme : public ITheme {
    public:
        GlbTheme();

        void drawTile(Vector3 pos, Vector3 size, bool isLight) const override;
        void drawResource(std::size_t resourceIndex, Vector3 pos, float height) const override;
        void drawPlayer(Vector3 pos, float rotationDeg) const override;
        void drawEgg(Vector3 pos) const override;

    private:
        CharacterModel _character;
        DefaultTheme _fallback;
};

}
