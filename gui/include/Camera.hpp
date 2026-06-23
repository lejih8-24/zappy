/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GameCamera wrapper around raylib Camera3D
*/

#pragma once

#include "raylib.h"

namespace GUI {

class GameCamera {
    public:
        GameCamera(Vector3 position, Vector3 target, float fovy = 45.0f);
        ~GameCamera() = default;

        // Call once per frame before begin3D() — handles mouse + scroll input
        void update();

        // Wrap BeginMode3D / EndMode3D
        void begin3D() const;
        void end3D()   const;

        // Reset to initial position/target
        void reset();

        // Returns true when cursor is captured (camera actively controlled)
        bool isCursorLocked() const;

        // Access underlying raylib camera if needed
        const Camera3D &get() const { return _camera; }

    private:
        Camera3D _camera;
        Vector3  _initPosition;
        Vector3  _initTarget;
};

}
