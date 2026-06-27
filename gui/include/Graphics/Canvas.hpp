/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Raylib drawing adapter
*/

#pragma once

#include "Graphics/Types.hpp"
#include <string_view>

namespace GUI {

class Canvas {
    public:
        int width() const;
        int height() const;
        int scaleSize(int value) const;
        int measureText(std::string_view text, int fontSize) const;

        void drawFPS(int x, int y) const;
        void drawText(std::string_view text, int x, int y, int fontSize, Color color) const;
        void drawLine(int startX, int startY, int endX, int endY, Color color) const;
        void drawRectangle(int x, int y, int width, int height, Color color) const;
        void drawRectangleLines(int x, int y, int width, int height, Color color) const;
        void drawCube(Vec3 pos, Vec3 size, Color color) const;
        void drawCubeWires(Vec3 pos, Vec3 size, Color color) const;
        void drawSphere(Vec3 pos, float radius, Color color) const;
        Color fade(Color color, float alpha) const;
};

}
