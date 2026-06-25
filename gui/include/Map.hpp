/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Map.hpp
*/

#pragma once

#include "Game/GameState.hpp"
#include "Theme/ITheme.hpp"

#include "raylib.h"

namespace GUI {

class Map {
    public:
        Map(ITheme &theme, float squareSize = 2.0f);
        ~Map() = default;

        void draw(const GameState &state) const;

    private:
        Vector3 getTilePosition(int x, int y, const GameState &state, float height = 0.0f) const;

        ITheme &_theme;
        float _squareSize;
};

}
