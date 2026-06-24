/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Map.hpp
*/

#pragma once

#include "Game/GameState.hpp"
#include "Game/CharacterModel.hpp"
#include "Game/EggModel.hpp"

#include "raylib.h"

namespace GUI {

class Map {
    public:
        Map(float squareSize = 2.0f);
        ~Map() = default;

        void draw(const GameState &state) const;

    private:
        Vector3 getTilePosition(int x, int y, const GameState &state, float height = 0.0f) const;

        float _squareSize;

        CharacterModel _character;
        EggModel _eggModel;
};

}
