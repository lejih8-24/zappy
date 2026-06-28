/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI render
*/

#include "Render.hpp"

#include "Game/GameStateEventHandler.hpp"
#include <networking.hpp>

#include <string>
#include <variant>

namespace GUI {

Render::Render(std::string_view host, int port, std::string_view pack, int width, int height)
    : _state()
    , _window(width, height,
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
        "[LClick] select player",
        "[X] quit",
        "[ESC] lock camera/settings",
    };
    const int fontSize = _canvas.scaleSize(20);
    const int lineSpacing = _canvas.scaleSize(28);
    const int startY = _canvas.height() / 2 + _canvas.scaleSize(55);

    for (std::size_t i = 0; i < sizeof(helpLines) / sizeof(helpLines[0]); ++i) {
        const int textWidth = _canvas.measureText(helpLines[i], fontSize);
        _canvas.drawText(helpLines[i], (_canvas.width() - textWidth) / 2,
            startY + static_cast<int>(i) * lineSpacing, fontSize, Colors::RayWhite);
    }
}

void Render::drawCameraLockLabel() const
{
    const char *lockLabel = _camera.isCursorLocked() ? "[ESC] lock camera/settings" : "[LOCKED] Click to unlock";

    _canvas.drawText(lockLabel, _canvas.scaleSize(10), _canvas.scaleSize(62), _canvas.scaleSize(18),
        _camera.isCursorLocked() ? Colors::Green : Colors::Yellow);
}

void Render::drawFocusOverlay() const
{
    if (_camera.isCursorLocked())
        return;

    _canvas.drawRectangle(0, 0, _canvas.width(), _canvas.height(), {0, 0, 0, 100});
    const char *msg = "Click to unlock";
    int fontSize = _canvas.scaleSize(36);
    int tw = _canvas.measureText(msg, fontSize);

    _canvas.drawText(msg, (_canvas.width() - tw) / 2, _canvas.height() / 2 - fontSize / 2, fontSize,
        Colors::RayWhite);
}

void Render::drawCrosshair() const
{
    if (!_camera.isCursorLocked())
        return;

    int cx = _canvas.width() / 2;
    int cy = _canvas.height() / 2;
    int crosshairSize = _canvas.scaleSize(12);

    _canvas.drawLine(cx - crosshairSize, cy, cx + crosshairSize, cy, Colors::RayWhite);
    _canvas.drawLine(cx, cy - crosshairSize, cx, cy + crosshairSize, Colors::RayWhite);
}

void Render::draw3DScene()
{
    _camera.begin3D();
    _map.draw(_canvas, _state, _window.now());
    _camera.end3D();
}

void Render::drawSelectedPlayerPanel() const
{
    if (!_selectedPlayerId.has_value())
        return;
    auto it = _state.players.find(*_selectedPlayerId);
    if (it != _state.players.end())
        _playerInfoPanel.draw(_canvas, it->second, _window.now());
}

void Render::drawFrame()
{
    _window.beginFrame(_themeManager.active().getBackgroundColor());
    _canvas.drawFPS(10, 10);

    draw3DScene();
    _map.drawLabels(_canvas, _state, _camera, _window.now());
    _hud.draw(_canvas, _state, _window.now());
    drawSelectedPlayerPanel();
    drawCrosshair();
    drawFocusOverlay();
    drawCameraLockLabel();
    drawHelpText();

    _window.endFrame();
}

void Render::handleGameInput()
{
    if (_window.isKeyPressed(Key::F11))
        _window.toggleFullscreen();
    if (_window.isKeyPressed(Key::H))
        _hud.update(HudAction::ToggleVisibility);
    if (_window.isKeyPressed(Key::Right))
        _hud.update(HudAction::NextPage);
    if (_window.isKeyPressed(Key::Left))
        _hud.update(HudAction::PreviousPage);
    if (_window.isKeyPressed(Key::Down))
        _hud.update(HudAction::ScrollDown);
    if (_window.isKeyPressed(Key::Up))
        _hud.update(HudAction::ScrollUp);

    if (_window.isMouseButtonPressed(MouseButton::Left) && _camera.isCursorLocked()) {
        int selected = -1;
        float closestDist = 1000.0f;

        for (const auto &[id, player] : _state.players) {
            Vec3 pos = _map.getPlayerWorldPos(player, _state, _window.now());
            pos.y += 0.6f;
            auto hitDistance = _camera.screenCenterHitDistance(pos, 0.6f);
            if (hitDistance && *hitDistance < closestDist) {
                closestDist = *hitDistance;
                selected = id;
            }
        }
        _selectedPlayerId = (selected != -1 && _selectedPlayerId != selected)
            ? std::optional<int>{selected} : std::optional<int>{};
    }

    if (_selectedPlayerId.has_value() && _state.players.find(*_selectedPlayerId) == _state.players.end())
        _selectedPlayerId = {};
}

void Render::pollServerEvents(Zappy::Networking::GraphicsClient &client)
{
    GameStateEventHandler handler(_state, _window.now());

    while (auto event = client.pollEvent())
        std::visit(handler, *event);
}

void Render::cleanupDeadPlayers()
{
    float now = _window.now();

    for (auto player = _state.players.begin(); player != _state.players.end();) {
        if (!player->second.alive && player->second.animState == Player::AnimState::Dead) {
            if (player->second.animStateEndTime <= 0.0F) {
                player->second.animStateEndTime = player->second.animStateStartTime
                    + _themeManager.active().getPlayerAnimationDuration(Player::AnimState::Dead);
            }
            if (now >= player->second.animStateEndTime) {
                if (_selectedPlayerId.has_value() && *_selectedPlayerId == player->first)
                    _selectedPlayerId = {};
                player = _state.players.erase(player);
                continue;
            }
        }
        ++player;
    }
}

void Render::update(Zappy::Networking::GraphicsClient &client)
{
    pollServerEvents(client);
    cleanupDeadPlayers();
    handleGameInput();
    _camera.update(_window);
}

void Render::renderLoop(Zappy::Networking::GraphicsClient &client)
{
    while (!_window.shouldClose()) {
        update(client);
        drawFrame();
    }
}

}
