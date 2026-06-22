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

Render::Render(std::string_view host, int port, GameState &state)
    : _state(state)
    , _window(1280, 720,
          std::string("Zappy GUI - ").append(host).append(":").append(std::to_string(port)), 60)
    , _map()
    , _camera({ 0.0f, 20.0f, 20.0f }, { 0.0f, 0.0f, 0.0f })
{
}

void Render::renderLoop(Zappy::Networking::GraphicsClient &client)
{
    GameStateEventHandler handler(_state);
    // May delete
    // double lastMove = GetTime();

    while (!_window.shouldClose()) {
        while (auto event = client.pollEvent())
            std::visit(handler, *event);
        if (IsKeyPressed(KEY_F11))
            _window.toggleFullscreen();

        // May delete
        // if (GetTime() - lastMove >= 1.0) {
        //     if (!_state.players.empty()) {
        //         auto &player = _state.players.begin()->second;
        //         player.x = (player.x + 1) % static_cast<int>(_state.mapWidth);
        //     }
        //     lastMove = GetTime();
        // }
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
