/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI renderer
*/

#include "Renderer.hpp"

#include "raylib.h"

namespace GUI {

Renderer::Renderer()
    : _window(1280, 720, "Zappy GUI", 60)
    , _map(10, 10)
    , _camera({ 0.0f, 20.0f, 20.0f }, { 0.0f, 0.0f, 0.0f })
{
}

void Renderer::renderLoop()
{
    while (!_window.shouldClose()) {
        _camera.update();

        _window.beginFrame();
        DrawFPS(10, 10);

        _camera.begin3D();
        _map.draw();
        _camera.end3D();

        DrawText("Zappy GUI - 3D Map  |  [WASD] deplacer  [R] reset", 10, 34, 18, RAYWHITE);
        _window.endFrame();
    }
}

}
