/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** 3D map grid
*/


#include "Map.hpp"

#include <array>

static constexpr float getResourceHeight(unsigned int quantity)
{
    if (quantity == 0)
        return 0.0F;
    const float height = 0.20F + 0.08F * static_cast<float>(quantity - 1);

    return height < 0.70F ? height : 0.70F;
}

// Prevents compilation if height formula is edited and gives an unexpected number
static_assert(getResourceHeight(0) == 0.0F); // No resource: nothing to draw
static_assert(getResourceHeight(1) == 0.20F); // One resource: minimum height
static_assert(getResourceHeight(100) == 0.70F); // Many resources: capped height

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
                const float height = getResourceHeight(quantity);
                Vector3 pos = getTilePosition(tile.x, tile.y, state, height / 2.0F);

                pos.x += resourceSlots[resourceIndex].x * _squareSize * resourceOffset;
                pos.z += resourceSlots[resourceIndex].y * _squareSize * resourceOffset;
                _theme.drawResource(resourceIndex, pos, height);
            }
            ++resourceIndex;
        }
    }

    for (const auto &[id, player] : state.players) {
        (void)id;
        Vector3 pos = getTilePosition(player.x, player.y, state, 0.0f);
        _theme.drawPlayer(pos, player.rotationDeg);
    }

    for (const auto &[id, egg] : state.eggs) {
        (void)id;
        Vector3 pos = getTilePosition(egg.x, egg.y, state, 0.35f);
        _theme.drawEgg(pos);
    }
}

}
