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

#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <variant>

// Count only incantations that are currently running (status activated)
static std::size_t getActiveIncantationCount(const GUI::GameState &state)
{
    return std::count_if(state.incantations.begin(), state.incantations.end(),
        [](const GUI::Incantation &incantation) {
            return incantation.active;
        });
}

// Count players whose their team name matches the teamName we're currently inspecting
static std::size_t getTeamPlayerCount(const GUI::GameState &state, std::string_view teamName)
{
    return std::count_if(state.players.begin(), state.players.end(), [teamName](const auto &entry) {
        return entry.second.teamName == teamName;
    });
}

// Build the HUD level summary for one team, like "L1:2 L3:1" (L[playerLvl]:[nb])
static std::string getTeamLevels(const GUI::GameState &state, std::string_view teamName)
{
    // lvl / nbPlayers
    std::map<std::size_t, std::size_t> levels;
    std::ostringstream stream;

    for (const auto &[id, player] : state.players) {
        (void)id;
        if (player.teamName == teamName)
            ++levels[player.level];
    }
    if (levels.empty())
        return "-";
    for (const auto &[level, nbPlayers] : levels)
        stream << "L" << level << ":" << nbPlayers << " ";
    std::string result = stream.str();
    result.pop_back(); // delete the useless space char at the end of the string
    return result;
}

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
        if (IsKeyPressed(KEY_H))
            _showHud = !_showHud;

        _camera.update();

        _window.beginFrame();
        DrawFPS(10, 10);

        _camera.begin3D();
        _map.draw(_state);
        _camera.end3D();

        if (_showHud)
            drawHud();
        if (_camera.isCursorLocked()) {
            int cx = GetScreenWidth() / 2;
            int cy = GetScreenHeight() / 2;
            DrawLine(cx - 12, cy, cx + 12, cy, RAYWHITE);
            DrawLine(cx, cy - 12, cx, cy + 12, RAYWHITE);
        }

        if (!_camera.isCursorLocked()) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});
            const char *msg = "Click to unlock";
            int tw = MeasureText(msg, 32);
            DrawText(msg, (GetScreenWidth() - tw) / 2, GetScreenHeight() / 2 - 16, 32, RAYWHITE);
        }

        const char *lockLabel = _camera.isCursorLocked() ? "[FREE]" : "[LOCKED] Click to unlock";
        DrawText(lockLabel, 10, 58, 16, _camera.isCursorLocked() ? GREEN : YELLOW);

        DrawText("Zappy GUI - 3D Map  |  [WASD] move  [R] reset  [F11] fullscreen  [H] HUD  |  [ESC] lock camera ", 10, 34, 18, RAYWHITE);
        _window.endFrame();
    }
}

void Render::drawHud() const
{
    static constexpr int panelWidth = 360;
    static constexpr int margin = 16;
    static constexpr int padding = 12;
    static constexpr int fontSize = 22;
    static constexpr int lineHeight = 27;
    const int x = _window.getWidth() - panelWidth - margin;
    const int y = margin;
    const int height = _window.getHeight() - margin * 2;
    int textY = y + padding;

    auto drawLine = [&](const std::string &line, Color color = RAYWHITE) {
        DrawText(line.c_str(), x + padding, textY, fontSize, color);
        textY += lineHeight;
    };

    DrawRectangle(x, y, panelWidth, height, Fade(BLACK, 0.65F));
    DrawRectangleLines(x, y, panelWidth, height, DARKGRAY);

    drawLine("HUD", YELLOW);
    drawLine("Map: " + std::to_string(_state.mapWidth) + "x" + std::to_string(_state.mapHeight));
    drawLine("Time: " + std::to_string(_state.timeUnit));
    drawLine("Players: " + std::to_string(_state.players.size()));
    drawLine("Eggs: " + std::to_string(_state.eggs.size()));
    drawLine("Teams: " + std::to_string(_state.teams.size()));
    drawLine("Incantations: " + std::to_string(getActiveIncantationCount(_state)) + "/"
        + std::to_string(_state.incantations.size()));
    drawLine(_state.gameEnded ? std::string("Winner: ") + _state.winningTeam : "State: Running");

    textY += lineHeight / 2;
    drawLine("Teams", YELLOW);
    for (const std::string &team : _state.teams) {
        const std::size_t playerCount = getTeamPlayerCount(_state, team);

        drawLine(team + ": " + std::to_string(playerCount) + " player" + (playerCount > 1 ? "s" : ""));
        drawLine("  " + getTeamLevels(_state, team), LIGHTGRAY);
    }
}

}
