/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI render
*/

#include "Render.hpp"

#include "Game/GameStateEventHandler.hpp"
#include "UI/Scale.hpp"
#include <networking.hpp>
#include "raylib.h"

#include <string>
#include <variant>

namespace GUI {

Render::Render(std::string_view host, int port)
    : _state()
    , _window(1440, 900,
          std::string("Zappy GUI - ").append(host).append(":").append(std::to_string(port)), 60)
    , _map()
    , _camera({ 0.0f, 20.0f, 20.0f }, { 0.0f, 0.0f, 0.0f })
    , _hud()
{
}

void Render::handleGameInput()
{
    if (IsKeyPressed(KEY_F11))
        _window.toggleFullscreen();
    if (IsKeyPressed(KEY_H))
        _hud.update(HudAction::ToggleVisibility);
    if (IsKeyPressed(KEY_RIGHT))
        _hud.update(HudAction::NextPage);
    if (IsKeyPressed(KEY_LEFT))
        _hud.update(HudAction::PreviousPage);
    if (IsKeyPressed(KEY_DOWN))
        _hud.update(HudAction::ScrollDown);
    if (IsKeyPressed(KEY_UP))
        _hud.update(HudAction::ScrollUp);
}

void Render::renderLoop(Zappy::Networking::GraphicsClient &client)
{
    GameStateEventHandler handler(_state);

    while (!_window.shouldClose()) {
        while (auto event = client.pollEvent())
            std::visit(handler, *event);

        handleGameInput();
        _camera.update();

        _window.beginFrame();
        DrawFPS(10, 10);

        _camera.begin3D();
        _map.draw(_state);
        _camera.end3D();

        _hud.draw(_state, _window);
        if (_camera.isCursorLocked()) {
            int cx = GetScreenWidth() / 2;
            int cy = GetScreenHeight() / 2;
            int crosshairSize = UI::scaleSize(12);
            DrawLine(cx - crosshairSize, cy, cx + crosshairSize, cy, RAYWHITE);
            DrawLine(cx, cy - crosshairSize, cx, cy + crosshairSize, RAYWHITE);
        }

        if (!_camera.isCursorLocked()) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});
            const char *msg = "Click to unlock";
            int fontSize = UI::scaleSize(36);
            int tw = MeasureText(msg, fontSize);
            DrawText(msg, (GetScreenWidth() - tw) / 2, GetScreenHeight() / 2 - fontSize / 2, fontSize, RAYWHITE);
        }

        const char *lockLabel = _camera.isCursorLocked() ? "[UNLOCKED]" : "[LOCKED] Click to unlock";
        DrawText(lockLabel, UI::scaleSize(10), UI::scaleSize(62), UI::scaleSize(18),
            _camera.isCursorLocked() ? GREEN : YELLOW);

        DrawText("Zappy GUI - 3D Map  |  [WASD] move  [R] reset  [F11] fullscreen  [H] HUD  [Arrows] HUD pages/scroll  [X] quit  |  [ESC] lock camera ",
            UI::scaleSize(10), UI::scaleSize(34), UI::scaleSize(20), RAYWHITE);
        _window.endFrame();
    }
}

}
