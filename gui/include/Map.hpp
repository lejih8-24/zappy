/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Map.hpp
*/

#pragma once

#include "Camera.hpp"
#include "Game/GameState.hpp"
#include "Graphics/Canvas.hpp"
#include "Theme/ITheme.hpp"

namespace GUI {

class Map {
    public:
        Map(ITheme &theme, float squareSize = 2.0f);
        ~Map() = default;

        void draw(const Canvas &canvas, const GameState &state, float currentTime) const;
        void drawLabels(const Canvas &canvas, const GameState &state, const GameCamera &camera, float currentTime) const;
        Vec3 getPlayerWorldPos(const Player &player, const GameState &state, float currentTime) const;

    private:
        Vec3 getTilePosition(float x, float y, const GameState &state, float height = 0.0f) const;
        void drawTiles(const Canvas &canvas, const GameState &state) const;
        void drawResources(const Canvas &canvas, const GameState &state) const;
        void drawPlayers(const Canvas &canvas, const GameState &state, float currentTime) const;
        void drawEggs(const Canvas &canvas, const GameState &state) const;
        void drawPlayerLabels(const Canvas &canvas, const GameState &state, const GameCamera &camera,
            float currentTime) const;
        void drawResourceLabels(const Canvas &canvas, const GameState &state, const GameCamera &camera) const;
        void drawCountLabel(const Canvas &canvas, Vec2 screenPos, int fontSize, unsigned int quantity) const;

        ITheme &_theme;
        float _squareSize;
};

}
