/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI render
*/

#include "Render.hpp"

#include "Game/GameStateEventHandler.hpp"
#include <networking.hpp>
#include "raylib.h"

#include <string>
#include <variant>

namespace GUI {

Render::Render(std::string_view host, int port)
    : _state()
    , _window(1280, 720,
          std::string("Zappy GUI - ").append(host).append(":").append(std::to_string(port)), 60)
    , _map()
    , _camera({ 0.0f, 20.0f, 20.0f }, { 0.0f, 0.0f, 0.0f })
{
}

void Render::renderLoop(Zappy::Networking::GraphicsClient &client)
{
    GameStateEventHandler handler(_state);

    while (!_window.shouldClose()) {
        while (auto event = client.pollEvent())
            std::visit(handler, *event);
        if (IsKeyPressed(KEY_F11))
            _window.toggleFullscreen();

        _camera.update();

        _window.beginFrame();
        DrawFPS(10, 10);

        _camera.begin3D();
        _map.draw(_state);
        _camera.end3D();

        if (_camera.isCursorLocked()) {
            int cx = GetScreenWidth() / 2;
            int cy = GetScreenHeight() / 2;
            DrawLine(cx - 12, cy, cx + 12, cy, RAYWHITE);
            DrawLine(cx, cy - 12, cx, cy + 12, RAYWHITE);
        }

        if (!_camera.isCursorLocked()) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});
            const char *msg = "Click to focus";
            int tw = MeasureText(msg, 32);
            DrawText(msg, (GetScreenWidth() - tw) / 2, GetScreenHeight() / 2 - 16, 32, RAYWHITE);
        }

        const char *lockLabel = _camera.isCursorLocked() ? "[LOCKED]  [ESC] to release" : "[FREE]  [Click] to grab";
        DrawText(lockLabel, 10, 58, 16, _camera.isCursorLocked() ? GREEN : YELLOW);

        DrawText("Zappy GUI - 3D Map  |  [WASD] deplacer  [R] reset  [F11] fullscreen", 10, 34, 18,
            RAYWHITE);
        _window.endFrame();
    }
}

}
