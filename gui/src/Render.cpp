/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI render
*/

#include "Render.hpp"

#include "raylib.h"

#include <string>

namespace GUI {

Render::Render(std::string_view host, int port, const GameState &state)
    : _state(state)
    , _window(1280, 720,
          std::string("Zappy GUI - ").append(host).append(":").append(std::to_string(port)), 60)
    , _map()
    , _camera({ 0.0f, 20.0f, 20.0f }, { 0.0f, 0.0f, 0.0f })
{
}

void Render::renderLoop()
{
    while (!_window.shouldClose()) {
        if (IsKeyPressed(KEY_F11))
            _window.toggleFullscreen();

        _camera.update();

        _window.beginFrame();
        DrawFPS(10, 10);

        _camera.begin3D();
        _map.draw(_state);
        _camera.end3D();

        DrawText("Zappy GUI - 3D Map  |  [WASD] deplacer  [R] reset  [F11] fullscreen", 10, 34, 18,
            RAYWHITE);
        _window.endFrame();
    }
}

}
