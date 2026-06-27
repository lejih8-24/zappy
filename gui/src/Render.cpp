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

Render::Render(std::string_view host, int port, std::string_view pack)
    : _state()
    , _window(1440, 900,
          std::string("Zappy GUI - ").append(host).append(":").append(std::to_string(port)), 60)
    , _themeManager(pack)
    , _map(_themeManager.active())
    , _camera({ 0.0f, 20.0f, 20.0f }, { 0.0f, 0.0f, 0.0f })
    , _hud()
{
}

void Render::drawHelpText() const
{
    if (_camera.isCursorLocked())
        return;

    static constexpr const char *helpLines[] = {
        "Zappy GUI - 3D Map",
        "[WASD] move",
        "[Shift] sprint",
        "[R] reset camera",
        "[F11] fullscreen",
        "[H] HUD",
        "[Arrows] HUD pages/scroll",
        "[X] quit",
        "[ESC] lock camera/settings",
    };
    const int fontSize = UI::scaleSize(20);
    const int lineSpacing = UI::scaleSize(28);
    const int startY = GetScreenHeight() / 2 + UI::scaleSize(55);

    for (std::size_t i = 0; i < sizeof(helpLines) / sizeof(helpLines[0]); ++i) {
        const int textWidth = MeasureText(helpLines[i], fontSize);
        DrawText(helpLines[i], (GetScreenWidth() - textWidth) / 2, startY + static_cast<int>(i) * lineSpacing,
            fontSize, RAYWHITE);
    }
}

void Render::drawCameraLockLabel() const
{
    const char *lockLabel = _camera.isCursorLocked() ? "[ESC] lock camera/settings" : "[LOCKED] Click to unlock";

    DrawText(lockLabel, UI::scaleSize(10), UI::scaleSize(62), UI::scaleSize(18),
        _camera.isCursorLocked() ? GREEN : YELLOW);
}

void Render::drawFocusOverlay() const
{
    if (_camera.isCursorLocked())
        return;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});
    const char *msg = "Click to unlock";
    int fontSize = UI::scaleSize(36);
    int tw = MeasureText(msg, fontSize);

    DrawText(msg, (GetScreenWidth() - tw) / 2, GetScreenHeight() / 2 - fontSize / 2, fontSize, RAYWHITE);
}

void Render::drawCrosshair() const
{
    if (!_camera.isCursorLocked())
        return;

    int cx = GetScreenWidth() / 2;
    int cy = GetScreenHeight() / 2;
    int crosshairSize = UI::scaleSize(12);

    DrawLine(cx - crosshairSize, cy, cx + crosshairSize, cy, RAYWHITE);
    DrawLine(cx, cy - crosshairSize, cx, cy + crosshairSize, RAYWHITE);
}

void Render::draw3DScene()
{
    _camera.begin3D();
    _map.draw(_state);
    _camera.end3D();
}

void Render::drawFrame()
{
    _window.beginFrame(_themeManager.active().getBackgroundColor());
    DrawFPS(10, 10);

    draw3DScene();
    _map.drawLabels(_state, _camera.get());
    _hud.draw(_state, _window);
    drawCrosshair();
    drawFocusOverlay();
    drawCameraLockLabel();
    drawHelpText();

    _window.endFrame();
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

void Render::pollServerEvents(Zappy::Networking::GraphicsClient &client)
{
    GameStateEventHandler handler(_state);

    while (auto event = client.pollEvent())
        std::visit(handler, *event);
}

void Render::update(Zappy::Networking::GraphicsClient &client)
{
    pollServerEvents(client);
    handleGameInput();
    _camera.update();
}

void Render::renderLoop(Zappy::Networking::GraphicsClient &client)
{
    while (!_window.shouldClose()) {
        update(client);
        drawFrame();
    }
}

}
