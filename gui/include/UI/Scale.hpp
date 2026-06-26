/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** UI scale helpers
*/

#pragma once

#include "raylib.h"

#include <algorithm>
#include <cmath>

namespace GUI::UI {

inline int scaleSize(int value)
{
    const float heightScale = static_cast<float>(GetScreenHeight()) / 900.0F;
    const float scale = std::max(1.0F, 1.0F + (heightScale - 1.0F) * 0.7F);

    return static_cast<int>(std::round(static_cast<float>(value) * scale));
}

}
