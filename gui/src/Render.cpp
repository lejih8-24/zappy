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
#include <array>
#include <cmath>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

static constexpr std::array<std::string_view, 4> hudPageNames = {
    "Overview",
    "Teams",
    "Players",
    "Resources",
};

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

static std::string getTeamLevelMinMax(const GUI::GameState &state, std::string_view teamName)
{
    std::size_t minLevel = 0;
    std::size_t maxLevel = 0;
    bool found = false;

    for (const auto &[id, player] : state.players) {
        (void)id;
        if (player.teamName != teamName)
            continue;
        if (!found) {
            minLevel = player.level;
            maxLevel = player.level;
            found = true;
            continue;
        }
        minLevel = std::min(minLevel, player.level);
        maxLevel = std::max(maxLevel, player.level);
    }
    if (!found)
        return "-";
    return std::to_string(minLevel) + "-" + std::to_string(maxLevel);
}

static std::string getPlayerFlags(const GUI::Player &player)
{
    std::string flags;

    if (player.isIncantating)
        flags += "I";
    if (player.isLayingEgg)
        flags += "E";
    if (player.isBroadcasting)
        flags += "B";
    return flags.empty() ? "-" : flags;
}

static void addOverviewLines(const GUI::GameState &state, std::vector<std::string> &lines)
{
    lines.push_back("Map: " + std::to_string(state.mapWidth) + "x" + std::to_string(state.mapHeight));
    lines.push_back("Time: " + std::to_string(state.timeUnit));
    lines.push_back("Players: " + std::to_string(state.players.size()));
    lines.push_back("Eggs: " + std::to_string(state.eggs.size()));
    lines.push_back("Teams: " + std::to_string(state.teams.size()));
    lines.push_back("Incantations: " + std::to_string(getActiveIncantationCount(state)) + "/"
        + std::to_string(state.incantations.size()));
    lines.push_back(state.gameEnded ? std::string("Winner: ") + state.winningTeam : "State: Running");
}

static void addTeamsLines(const GUI::GameState &state, std::vector<std::string> &lines)
{
    for (const std::string &team : state.teams) {
        const std::size_t playerCount = getTeamPlayerCount(state, team);

        lines.push_back(team + ": " + std::to_string(playerCount) + " player" + (playerCount > 1 ? "s" : ""));
        lines.push_back("  levels: " + getTeamLevels(state, team));
        lines.push_back("  min/max: " + getTeamLevelMinMax(state, team));
    }
    if (state.teams.empty())
        lines.push_back("No team");
}

static void addPlayersForTeam(const GUI::GameState &state, std::vector<std::string> &lines, std::string_view teamName)
{
    bool found = false;

    for (const auto &[id, player] : state.players) {
        if (player.teamName != teamName)
            continue;
        found = true;
        lines.push_back("  #" + std::to_string(id) + " L" + std::to_string(player.level) + " ("
            + std::to_string(player.x) + "," + std::to_string(player.y) + ") "
            + std::to_string(static_cast<int>(player.rotationDeg)) + "deg " + getPlayerFlags(player));
    }
    if (!found)
        lines.push_back("  no player");
}

static void addPlayersLines(const GUI::GameState &state, std::vector<std::string> &lines)
{
    for (const std::string &team : state.teams) {
        lines.push_back(team);
        addPlayersForTeam(state, lines, team);
    }
    for (const auto &[id, player] : state.players) {
        if (std::find(state.teams.begin(), state.teams.end(), player.teamName) != state.teams.end())
            continue;
        lines.push_back(player.teamName.empty() ? "No team" : player.teamName);
        lines.push_back("  #" + std::to_string(id) + " L" + std::to_string(player.level) + " ("
            + std::to_string(player.x) + "," + std::to_string(player.y) + ") "
            + std::to_string(static_cast<int>(player.rotationDeg)) + "deg " + getPlayerFlags(player));
    }
    if (state.players.empty())
        lines.push_back("No player");
}

static void addResourceLine(std::vector<std::string> &lines, std::string_view name,
    unsigned int mapQuantity, unsigned int playerQuantity)
{
    lines.push_back(std::string(name) + ": map=" + std::to_string(mapQuantity)
        + " players=" + std::to_string(playerQuantity)
        + " total=" + std::to_string(mapQuantity + playerQuantity));
}

static void addResourcesLines(const GUI::GameState &state, std::vector<std::string> &lines)
{
    Zappy::Game::Resources mapResources = {};
    Zappy::Game::Resources playerResources = {};

    for (const GUI::Tile &tile : state.tiles)
        mapResources += tile.resources;
    for (const auto &[id, player] : state.players) {
        (void)id;
        playerResources += player.inventory;
    }
    addResourceLine(lines, "Food", mapResources.food, playerResources.food);
    addResourceLine(lines, "Linemate", mapResources.linemate, playerResources.linemate);
    addResourceLine(lines, "Deraumere", mapResources.deraumere, playerResources.deraumere);
    addResourceLine(lines, "Sibur", mapResources.sibur, playerResources.sibur);
    addResourceLine(lines, "Mendiane", mapResources.mendiane, playerResources.mendiane);
    addResourceLine(lines, "Phiras", mapResources.phiras, playerResources.phiras);
    addResourceLine(lines, "Thystame", mapResources.thystame, playerResources.thystame);
}

static std::vector<std::string> getHudLines(const GUI::GameState &state, std::size_t page)
{
    std::vector<std::string> lines;

    if (page == 0)
        addOverviewLines(state, lines);
    if (page == 1)
        addTeamsLines(state, lines);
    if (page == 2)
        addPlayersLines(state, lines);
    if (page == 3)
        addResourcesLines(state, lines);
    return lines;
}

static int scaleSize(int value)
{
    const float heightScale = static_cast<float>(GetScreenHeight()) / 900.0F;
    const float scale = std::max(1.0F, 1.0F + (heightScale - 1.0F) * 0.7F);

    return static_cast<int>(std::round(static_cast<float>(value) * scale));
}

namespace GUI {

Render::Render(std::string_view host, int port)
    : _state()
    , _window(1440, 900,
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
        if (_showHud && IsKeyPressed(KEY_RIGHT)) {
            _hudPage = (_hudPage + 1) % hudPageNames.size();
            _hudScroll = 0;
        }
        if (_showHud && IsKeyPressed(KEY_LEFT)) {
            _hudPage = (_hudPage + hudPageNames.size() - 1) % hudPageNames.size();
            _hudScroll = 0;
        }
        if (_showHud && IsKeyPressed(KEY_DOWN))
            ++_hudScroll;
        if (_showHud && IsKeyPressed(KEY_UP) && _hudScroll > 0)
            --_hudScroll;

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
            int crosshairSize = scaleSize(12);
            DrawLine(cx - crosshairSize, cy, cx + crosshairSize, cy, RAYWHITE);
            DrawLine(cx, cy - crosshairSize, cx, cy + crosshairSize, RAYWHITE);
        }

        if (!_camera.isCursorLocked()) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});
            const char *msg = "Click to unlock";
            int fontSize = scaleSize(36);
            int tw = MeasureText(msg, fontSize);
            DrawText(msg, (GetScreenWidth() - tw) / 2, GetScreenHeight() / 2 - fontSize / 2, fontSize, RAYWHITE);
        }

        const char *lockLabel = _camera.isCursorLocked() ? "[UNLOCKED]" : "[LOCKED] Click to unlock";
        DrawText(lockLabel, scaleSize(10), scaleSize(62), scaleSize(18), _camera.isCursorLocked() ? GREEN : YELLOW);

        DrawText("Zappy GUI - 3D Map  |  [WASD] move  [R] reset  [F11] fullscreen  [H] HUD  [Arrows] HUD pages/scroll  [X] quit  |  [ESC] lock camera ",
            scaleSize(10), scaleSize(34), scaleSize(20), RAYWHITE);
        _window.endFrame();
    }
}

void Render::drawHud()
{
    const int panelWidth = scaleSize(360);
    const int margin = scaleSize(16);
    const int padding = scaleSize(12);
    const int fontSize = scaleSize(22);
    const int lineHeight = scaleSize(27);
    const int x = _window.getWidth() - panelWidth - margin;
    const int y = margin;
    const int height = _window.getHeight() - margin * 2;
    const int titleY = y + padding;
    const int contentY = titleY + lineHeight + lineHeight / 2;
    const int footerY = y + height - padding - lineHeight;
    const int visibleLines = std::max(1, (footerY - contentY - lineHeight / 2) / lineHeight);
    std::vector<std::string> lines = getHudLines(_state, _hudPage);
    int textY = contentY;
    const std::size_t maxScroll = lines.size() > static_cast<std::size_t>(visibleLines)
        ? lines.size() - static_cast<std::size_t>(visibleLines)
        : 0;

    if (_hudScroll > maxScroll)
        _hudScroll = maxScroll;

    const std::size_t end = std::min(lines.size(), _hudScroll + static_cast<std::size_t>(visibleLines));

    // Draw HUD background panel
    DrawRectangle(x, y, panelWidth, height, Fade(BLACK, 0.65F));
    DrawRectangleLines(x, y, panelWidth, height, DARKGRAY);

    // Draw HUD page title
    DrawText((std::string("HUD ") + std::to_string(_hudPage + 1) + "/" + std::to_string(hudPageNames.size())
        + " - " + std::string(hudPageNames[_hudPage])).c_str(), x + padding, titleY, fontSize, YELLOW);

    // Draw visible lines from the current page on the HUD
    for (std::size_t i = _hudScroll; i < end; ++i) {
        DrawText(lines[i].c_str(), x + padding, textY, fontSize, RAYWHITE);
        textY += lineHeight;
    }

    // Draw HUD navigation footer
    DrawText((std::string("<- page | scroll ")
        + std::to_string(_hudScroll) + "/" + std::to_string(maxScroll)
        + " | page ->").c_str(), x + padding, footerY, fontSize, LIGHTGRAY);
}

}
