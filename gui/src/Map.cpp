/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** 3D map grid
*/


#include "Map.hpp"

namespace GUI {

Map::Map(float squareSize)
    : _squareSize(squareSize)
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
    // Draw the map grid from the current GameState size.
    for (std::size_t row = 0; row < state.mapHeight; ++row) {
        for (std::size_t col = 0; col < state.mapWidth; ++col) {
            Vector3 pos = getTilePosition(static_cast<int>(col), static_cast<int>(row), state, -0.1f);
            Vector3 size = { _squareSize - 0.1f, 0.2f, _squareSize - 0.1f };
            Color squareColor = ((col + row) % 2 == 0) ? GREEN : DARKGREEN;

            DrawCubeV(pos, size, squareColor);
            DrawCubeWiresV(pos, size, BLACK);
        }
    }

    // Draw one marker on tiles that contain at least one resource.
    for (const Tile &tile : state.tiles) {
        unsigned int resourceCount = 0;

        for (unsigned int quantity : tile.resources)
            resourceCount += quantity;
        if (resourceCount > 0) {
            Vector3 pos = getTilePosition(tile.x, tile.y, state, 0.35f);
            DrawCubeV(pos, { 0.45f, 0.55f, 0.45f }, GOLD);
        }
    }

    // Draw players from the GameState instead of hardcoded render data.
    for (const auto &[id, player] : state.players) {
        (void)id;
        Vector3 pos = getTilePosition(player.x, player.y, state, 0.65f);
        DrawCubeV(pos, { 0.75f, 1.1f, 0.75f }, BLUE);
    }

    // Draw eggs from the GameState.
    for (const auto &[id, egg] : state.eggs) {
        (void)id;
        Vector3 pos = getTilePosition(egg.x, egg.y, state, 0.35f);
        DrawSphere(pos, 0.35f, RAYWHITE);
    }
}

}
