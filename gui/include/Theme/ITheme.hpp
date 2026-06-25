/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Abstract theme interface for the pack system
*/

#pragma once

#include "raylib.h"
#include <cstddef>

namespace GUI {

class ITheme {
    public:
        virtual ~ITheme() = default;

        virtual void drawTile(Vector3 pos, Vector3 size, bool isLight) const = 0;
        virtual void drawResource(std::size_t resourceIndex, Vector3 pos, float height) const = 0;
        virtual void drawPlayer(Vector3 pos, float rotationDeg) const = 0;
        virtual void drawEgg(Vector3 pos) const = 0;
};

}
