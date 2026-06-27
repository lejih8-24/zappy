/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** 3D map grid
*/


#include "Map.hpp"

#include <algorithm>
#include <array>
#include <optional>
#include <string>
#include <raymath.h>

// Y size of every resource model - center at half height above tile surface
static constexpr float RESOURCE_HEIGHT = 0.20F;

// Returns nullopt if worldPos is behind the camera or outside screen bounds
static std::optional<Vector2> projectToScreen(Vector3 worldPos, Camera3D camera, Vector3 camForward)
{
    // dot product <= 0 means the point is behind or perpendicular to the camera
    if (Vector3DotProduct(camForward, Vector3Subtract(worldPos, camera.position)) <= 0)
        return {};
    Vector2 screen = GetWorldToScreen(worldPos, camera);
    if (screen.x < 0 || screen.x > static_cast<float>(GetScreenWidth()) ||
        screen.y < 0 || screen.y > static_cast<float>(GetScreenHeight()))
        return {};
    return screen;
}

namespace GUI {

Map::Map(ITheme &theme, float squareSize)
    : _theme(theme)
    , _squareSize(squareSize)
{
}

// Centers the grid on the world origin so the map is symmetrical around (0,0)
Vector3 Map::getTilePosition(float x, float y, const GameState &state, float height) const
{
    return {
        (x - static_cast<float>(state.mapWidth) / 2.0f) * _squareSize,
        height,
        (y - static_cast<float>(state.mapHeight) / 2.0f) * _squareSize,
    };
}

void Map::drawTiles(const GameState &state) const
{
    for (std::size_t row = 0; row < state.mapHeight; ++row) {
        for (std::size_t col = 0; col < state.mapWidth; ++col) {
            // Y = -0.1f so the tile sits just below Y=0 where players stand
            Vector3 pos = getTilePosition(static_cast<int>(col), static_cast<int>(row), state, -0.1f);
            Vector3 size = { _squareSize - 0.1f, 0.2f, _squareSize - 0.1f };
            _theme.drawTile(pos, size, (col + row) % 2 == 0);
        }
    }
}

void Map::drawResources(const GameState &state) const
{
    // Fraction of squareSize used to spread each resource slot from tile center
    static constexpr float resourceOffset = 0.28F;
    static constexpr std::array<Vector2, Zappy::Game::Resources::RESOURCE_COUNT> resourceSlots = {
        Vector2{-1.0F, -1.0F}, //? Food
        Vector2{0.0F, -1.0F},  //? Linemate
        Vector2{1.0F, -1.0F},  //? Deraumere
        Vector2{-1.0F, 0.0F},  //? Sibur
        Vector2{1.0F, 0.0F},   //? Mendiane
        Vector2{-1.0F, 1.0F},  //? Phiras
        Vector2{0.0F, 1.0F},   //? Thystame
    };

    for (const Tile &tile : state.tiles) {
        std::size_t resourceIndex = 0;
        for (unsigned int quantity : tile.resources) {
            if (quantity > 0) {
                // Y = half height so the model sits on the tile surface
                Vector3 pos = getTilePosition(tile.x, tile.y, state, RESOURCE_HEIGHT / 2.0F);
                pos.x += resourceSlots[resourceIndex].x * _squareSize * resourceOffset;
                pos.z += resourceSlots[resourceIndex].y * _squareSize * resourceOffset;
                _theme.drawResource(resourceIndex, pos);
            }
            ++resourceIndex;
        }
    }
}

void Map::drawPlayers(const GameState &state) const
{
    float now = GetTime();
    for (const auto &[id, player] : state.players) {
        (void)id;
        Player::DisplayPosition displayPos = player.getDisplayPosition(now);
        Vector3 pos = getTilePosition(displayPos.x, displayPos.y, state, 0.0f);
        Player::AnimState animState = player.getEffectiveAnimState(now);
        _theme.drawPlayer(pos, player.rotationDeg, animState, player.getAnimationElapsed(now, animState));
    }
}

void Map::drawEggs(const GameState &state) const
{
    for (const auto &[id, egg] : state.eggs) {
        (void)id;
        Vector3 pos = getTilePosition(egg.x, egg.y, state, 0.35f);
        _theme.drawEgg(pos);
    }
}

void Map::draw(const GameState &state) const
{
    drawTiles(state);
    drawResources(state);
    drawPlayers(state);
    drawEggs(state);
}

void Map::drawCountLabel(Vector2 screenPos, int fontSize, unsigned int quantity) const
{
    std::string label = "x" + std::to_string(quantity);
    int textWidth = MeasureText(label.c_str(), fontSize);
    int sx = static_cast<int>(screenPos.x) - textWidth / 2;
    int sy = static_cast<int>(screenPos.y);
    DrawText(label.c_str(), sx + 1, sy + 1, fontSize, BLACK); // drop shadow
    DrawText(label.c_str(), sx, sy, fontSize, YELLOW);
}

void Map::drawResourceLabels(const GameState &state, Camera3D camera, Vector3 camForward) const
{
    static constexpr float resourceOffset = 0.28F;
    static constexpr std::array<Vector2, Zappy::Game::Resources::RESOURCE_COUNT> resourceSlots = {
        Vector2{-1.0F, -1.0F}, //? Food
        Vector2{0.0F, -1.0F},  //? Linemate
        Vector2{1.0F, -1.0F},  //? Deraumere
        Vector2{-1.0F, 0.0F},  //? Sibur
        Vector2{1.0F, 0.0F},   //? Mendiane
        Vector2{-1.0F, 1.0F},  //? Phiras
        Vector2{0.0F, 1.0F},   //? Thystame
    };

    for (const Tile &tile : state.tiles) {
        std::size_t resourceIndex = 0;
        for (unsigned int quantity : tile.resources) {
            if (quantity > 1) {
                // Float label above the resource model
                Vector3 worldPos = getTilePosition(tile.x, tile.y, state, RESOURCE_HEIGHT + 0.3f);
                worldPos.x += resourceSlots[resourceIndex].x * _squareSize * resourceOffset;
                worldPos.z += resourceSlots[resourceIndex].y * _squareSize * resourceOffset;
                auto screenPos = projectToScreen(worldPos, camera, camForward);
                if (screenPos) {
                    float dist = Vector3Distance(camera.position, worldPos);
                    // Font scales inversely with distance, clamped to readable range
                    int fontSize = std::clamp(static_cast<int>(80.0f / dist), 6, 16);
                    drawCountLabel(*screenPos, fontSize, quantity);
                }
            }
            ++resourceIndex;
        }
    }
}

void Map::drawPlayerLabels(const GameState &state, Camera3D camera, Vector3 camForward) const
{
    float now = GetTime();
    for (const auto &[id, player] : state.players) {
        (void)id;
        Player::DisplayPosition displayPos = player.getDisplayPosition(now);
        Vector3 labelPos = getTilePosition(displayPos.x, displayPos.y, state, _theme.getPlayerLabelHeight());

        auto screenPos = projectToScreen(labelPos, camera, camForward);
        if (!screenPos)
            continue;

        float dist = Vector3Distance(camera.position, labelPos);
        // Font scales inversely with distance, clamped to readable range
        int fontSize = std::clamp(static_cast<int>(_theme.getPlayerLabelScale() / dist), 8, 22);
        std::string label = player.teamName + " L" + std::to_string(player.level);
        int textWidth = MeasureText(label.c_str(), fontSize);
        int sx = static_cast<int>(screenPos->x) - textWidth / 2;
        int sy = static_cast<int>(screenPos->y);

        DrawText(label.c_str(), sx + 1, sy + 1, fontSize, BLACK); // drop shadow
        DrawText(label.c_str(), sx, sy, fontSize, RAYWHITE);
    }
}

Vector3 Map::getPlayerWorldPos(const Player &player, const GameState &state) const
{
    Player::DisplayPosition displayPos = player.getDisplayPosition(GetTime());
    return getTilePosition(displayPos.x, displayPos.y, state, 0.0f);
}

void Map::drawLabels(const GameState &state, Camera3D camera) const
{
    Vector3 camForward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    drawResourceLabels(state, camera, camForward);
    drawPlayerLabels(state, camera, camForward);
}

}
