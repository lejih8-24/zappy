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
        void drawLabels(const GameState &state, Camera3D camera) const;
        Vector3 getPlayerWorldPos(const Player &player, const GameState &state) const;

    private:
        Vector3 getTilePosition(float x, float y, const GameState &state, float height = 0.0f) const;
        void drawTiles(const GameState &state) const;
        void drawResources(const GameState &state) const;
        void drawPlayers(const GameState &state) const;
        void drawEggs(const GameState &state) const;
        void drawPlayerLabels(const GameState &state, Camera3D camera, Vector3 camForward) const;
        void drawResourceLabels(const GameState &state, Camera3D camera, Vector3 camForward) const;
        void drawCountLabel(Vector2 screenPos, int fontSize, unsigned int quantity) const;

        ITheme &_theme;
        float _squareSize;
};

}
