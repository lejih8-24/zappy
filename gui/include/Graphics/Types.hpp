/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI graphics value types
*/

#pragma once

namespace GUI {

struct Vec2 {
    float x = 0.0F;
    float y = 0.0F;
};

struct Vec3 {
    float x = 0.0F;
    float y = 0.0F;
    float z = 0.0F;
};

struct Color {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 255;
};

namespace Colors {
    inline constexpr Color Black = {0, 0, 0, 255};
    inline constexpr Color DarkBlue = {0, 82, 172, 255};
    inline constexpr Color DarkGray = {80, 80, 80, 255};
    inline constexpr Color DarkGreen = {0, 117, 44, 255};
    inline constexpr Color Green = {0, 228, 48, 255};
    inline constexpr Color Blue = {0, 121, 241, 255};
    inline constexpr Color Red = {230, 41, 55, 255};
    inline constexpr Color Yellow = {253, 249, 0, 255};
    inline constexpr Color Orange = {255, 161, 0, 255};
    inline constexpr Color Purple = {112, 31, 126, 255};
    inline constexpr Color Violet = {135, 60, 190, 255};
    inline constexpr Color Pink = {255, 109, 194, 255};
    inline constexpr Color SkyBlue = {102, 191, 255, 255};
    inline constexpr Color LightGray = {200, 200, 200, 255};
    inline constexpr Color RayWhite = {245, 245, 245, 255};
    inline constexpr Color Brown = {127, 106, 79, 255};
    inline constexpr Color Magenta = {255, 0, 255, 255};
}

}
