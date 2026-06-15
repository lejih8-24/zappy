/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Map.hpp
*/

#pragma once

#include "raylib.h"

namespace GUI {

class Map {
    private:
        int _width;
        int _height;
        float _squareSize;

    public:
        Map(int width, int height, float squareSize = 2.0f);
        ~Map() = default;

        void draw() const;
        void setSize(int width, int height);

        int getWidth() const
        {
            return _width;
        }

        int getHeight() const
        {
            return _height;
        }
};

}
