/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** 3D map grid
*/


#include "Map.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <raymath.h>

static constexpr float RESOURCE_HEIGHT = 0.20F;

namespace GUI {

Map::Map(ITheme &theme, float squareSize)
    : _theme(theme)
    , _squareSize(squareSize)
{
}

Vector3 Map::getTilePosition(int x, int y, const GameState &state, float height) const
{
    return {
        (static_cast<float>(x) - static_cast<float>(state.mapWidth) / 2.0f) * _squareSize,
        height,
        (static_cast<float>(y) - static_cast<float>(state.mapHeight) / 2.0f) * _squareSize,
    };
}

void Map::draw(const GameState &state) const
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

    for (std::size_t row = 0; row < state.mapHeight; ++row) {
        for (std::size_t col = 0; col < state.mapWidth; ++col) {
            Vector3 pos = getTilePosition(static_cast<int>(col), static_cast<int>(row), state, -0.1f);
            Vector3 size = { _squareSize - 0.1f, 0.2f, _squareSize - 0.1f };
            _theme.drawTile(pos, size, (col + row) % 2 == 0);
        }
    }

    for (const Tile &tile : state.tiles) {
        std::size_t resourceIndex = 0;

        for (unsigned int quantity : tile.resources) {
            if (quantity > 0) {
                Vector3 pos = getTilePosition(tile.x, tile.y, state, RESOURCE_HEIGHT / 2.0F);
                pos.x += resourceSlots[resourceIndex].x * _squareSize * resourceOffset;
                pos.z += resourceSlots[resourceIndex].y * _squareSize * resourceOffset;
                _theme.drawResource(resourceIndex, pos);
            }
            ++resourceIndex;
        }
    }

    float now = GetTime();
    for (const auto &[id, player] : state.players) {
        (void)id;
        Vector3 pos = getTilePosition(player.x, player.y, state, 0.0f);
        _theme.drawPlayer(pos, player.rotationDeg, player.getEffectiveAnimState(now));
    }

    for (const auto &[id, egg] : state.eggs) {
        (void)id;
        Vector3 pos = getTilePosition(egg.x, egg.y, state, 0.35f);
        _theme.drawEgg(pos);
    }
}

void Map::drawResourceLabels(const GameState &state, Camera3D camera, Vector3 camForward) const
{
    static constexpr float resourceOffset = 0.28F;
    static constexpr std::array<Vector2, Zappy::Game::Resources::RESOURCE_COUNT> resourceSlots = {
        Vector2{-1.0F, -1.0F}, Vector2{0.0F, -1.0F}, Vector2{1.0F, -1.0F},
        Vector2{-1.0F, 0.0F},  Vector2{1.0F, 0.0F},
        Vector2{-1.0F, 1.0F},  Vector2{0.0F, 1.0F},
    };

    for (const Tile &tile : state.tiles) {
        std::size_t resourceIndex = 0;
        for (unsigned int quantity : tile.resources) {
            if (quantity > 1) {
                Vector3 worldPos = getTilePosition(tile.x, tile.y, state, RESOURCE_HEIGHT + 0.3f);
                worldPos.x += resourceSlots[resourceIndex].x * _squareSize * resourceOffset;
                worldPos.z += resourceSlots[resourceIndex].y * _squareSize * resourceOffset;

                if (Vector3DotProduct(camForward, Vector3Subtract(worldPos, camera.position)) > 0) {
                    Vector2 screenPos = GetWorldToScreen(worldPos, camera);
                    if (screenPos.x >= 0 && screenPos.x <= static_cast<float>(GetScreenWidth()) &&
                        screenPos.y >= 0 && screenPos.y <= static_cast<float>(GetScreenHeight())) {
                        float dist = Vector3Distance(camera.position, worldPos);
                        int fontSize = std::clamp(static_cast<int>(80.0f / dist), 6, 16);
                        std::string label = "x" + std::to_string(quantity);
                        int textWidth = MeasureText(label.c_str(), fontSize);
                        int sx = static_cast<int>(screenPos.x) - textWidth / 2;
                        int sy = static_cast<int>(screenPos.y);
                        DrawText(label.c_str(), sx + 1, sy + 1, fontSize, BLACK);
                        DrawText(label.c_str(), sx, sy, fontSize, YELLOW);
                    }
                }
            }
            ++resourceIndex;
        }
    }
}

void Map::drawPlayerLabels(const GameState &state, Camera3D camera, Vector3 camForward) const
{
    for (const auto &[id, player] : state.players) {
        (void)id;
        Vector3 labelPos = getTilePosition(player.x, player.y, state, _theme.getPlayerLabelHeight());

        if (Vector3DotProduct(camForward, Vector3Subtract(labelPos, camera.position)) <= 0)
            continue;

        Vector2 screenPos = GetWorldToScreen(labelPos, camera);

        if (screenPos.x < 0 || screenPos.x > static_cast<float>(GetScreenWidth()) ||
            screenPos.y < 0 || screenPos.y > static_cast<float>(GetScreenHeight()))
            continue;

        float dist = Vector3Distance(camera.position, labelPos);
        int fontSize = std::clamp(static_cast<int>(_theme.getPlayerLabelScale() / dist), 8, 22);

        std::string label = player.teamName + " L" + std::to_string(player.level);
        int textWidth = MeasureText(label.c_str(), fontSize);
        int sx = static_cast<int>(screenPos.x) - textWidth / 2;
        int sy = static_cast<int>(screenPos.y);

        DrawText(label.c_str(), sx + 1, sy + 1, fontSize, BLACK);
        DrawText(label.c_str(), sx, sy, fontSize, RAYWHITE);
    }
}

}
