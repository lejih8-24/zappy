/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Raylib drawing adapter
*/

#include "Graphics/Canvas.hpp"

#include "raylib.h"

#include <algorithm>
#include <cmath>
#include <string>

static ::Color toRaylibColor(GUI::Color color)
{
    return {color.r, color.g, color.b, color.a};
}

static ::Vector3 toRaylibVec3(GUI::Vec3 value)
{
    return {value.x, value.y, value.z};
}

namespace GUI {

int Canvas::width() const
{
    return GetScreenWidth();
}

int Canvas::height() const
{
    return GetScreenHeight();
}

int Canvas::scaleSize(int value) const
{
    const float heightScale = static_cast<float>(height()) / 900.0F;
    const float scale = std::max(1.0F, 1.0F + (heightScale - 1.0F) * 0.7F);

    return static_cast<int>(std::round(static_cast<float>(value) * scale));
}

int Canvas::measureText(std::string_view text, int fontSize) const
{
    return MeasureText(std::string(text).c_str(), fontSize);
}

void Canvas::drawFPS(int x, int y) const
{
    DrawFPS(x, y);
}

void Canvas::drawText(std::string_view text, int x, int y, int fontSize, Color color) const
{
    DrawText(std::string(text).c_str(), x, y, fontSize, toRaylibColor(color));
}

void Canvas::drawLine(int startX, int startY, int endX, int endY, Color color) const
{
    DrawLine(startX, startY, endX, endY, toRaylibColor(color));
}

void Canvas::drawRectangle(int x, int y, int width, int height, Color color) const
{
    DrawRectangle(x, y, width, height, toRaylibColor(color));
}

void Canvas::drawRectangleLines(int x, int y, int width, int height, Color color) const
{
    DrawRectangleLines(x, y, width, height, toRaylibColor(color));
}

void Canvas::drawCube(Vec3 pos, Vec3 size, Color color) const
{
    DrawCubeV(toRaylibVec3(pos), toRaylibVec3(size), toRaylibColor(color));
}

void Canvas::drawCubeWires(Vec3 pos, Vec3 size, Color color) const
{
    DrawCubeWiresV(toRaylibVec3(pos), toRaylibVec3(size), toRaylibColor(color));
}

void Canvas::drawSphere(Vec3 pos, float radius, Color color) const
{
    DrawSphere(toRaylibVec3(pos), radius, toRaylibColor(color));
}

Color Canvas::fade(Color color, float alpha) const
{
    ::Color faded = Fade(toRaylibColor(color), alpha);
    return {faded.r, faded.g, faded.b, faded.a};
}

}
