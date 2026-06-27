/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GameCamera wrapper
*/

#pragma once

#include "Graphics/Types.hpp"
#include "Window.hpp"

#include <memory>
#include <optional>

namespace GUI {

class GameCamera {
    public:
        GameCamera(Vec3 position, Vec3 target, float fovy = 45.0f);
        ~GameCamera();

        // Call once per frame before begin3D() — handles mouse + scroll input
        void update(const Window &window);

        void begin3D() const;
        void end3D()   const;

        // Reset to initial position/target
        void reset();

        // Returns true when cursor is captured (camera actively controlled)
        bool isCursorLocked() const;

        std::optional<Vec2> projectToScreen(Vec3 worldPos) const;
        float distanceTo(Vec3 worldPos) const;
        std::optional<float> screenCenterHitDistance(Vec3 worldPos, float radius) const;

    private:
        struct CameraData;

        void updateFreeCamera(const Window &window);
        void moveCamera(Vec3 direction, float distance);
        Vec3 getForwardVector() const;
        Vec3 getRightVector() const;

        std::unique_ptr<CameraData> _cameraData;
};

}
