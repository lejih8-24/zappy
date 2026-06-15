/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** 3D map grid
*/

#include "Map.hpp"

namespace GUI {

Map::Map(int width, int height, float squareSize)
    : _width(width)
    , _height(height)
    , _squareSize(squareSize)
{
}

void Map::setSize(int width, int height)
{
    _width  = width;
    _height = height;
}

void Map::draw() const
{
    for (int row = 0; row < _height; ++row) {
        for (int col = 0; col < _width; ++col) {

            float x = (col - _width  / 2.0f) * _squareSize;
            float z = (row - _height / 2.0f) * _squareSize;

            Vector3 pos  = { x, -0.1f, z };
            Vector3 size = { _squareSize - 0.1f, 0.2f, _squareSize - 0.1f };

            Color squareColor = ((col + row) % 2 == 0) ? GREEN : DARKGREEN;

            DrawCubeV(pos, size, squareColor);
            DrawCubeWiresV(pos, size, BLACK);
        }
    }
}

}
