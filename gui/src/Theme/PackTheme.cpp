/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Generic pack theme with three-level asset fallback chain
*/

#include "Theme/PackTheme.hpp"

#include <cmath>
#include <filesystem>
#include <string>

static constexpr int WALK_ANIM_INDEX = 2;
static constexpr float ANIM_FPS = 24.0f;
static constexpr const char *BASE_PACK = "green_man";

static std::string resolvePath(std::string_view packName, const char *filename)
{
    std::string path = std::string(PACKS_DIR) + std::string(packName) + "/" + filename;

    if (std::filesystem::exists(path))
        return path;
    std::string basePath = std::string(PACKS_DIR) + BASE_PACK + "/" + filename;
    if (std::filesystem::exists(basePath))
        return basePath;
    return {};
}

namespace GUI {

PackTheme::PackTheme(std::string_view packName)
{
    std::string playerPath = resolvePath(packName, "player.glb");
    if (!playerPath.empty()) {
        try {
            _player = std::make_unique<CharacterModel>(playerPath);
        } catch (...) {
            _player = nullptr;
        }
    }

    std::string eggPath = resolvePath(packName, "egg.glb");
    if (!eggPath.empty())
        _egg = LoadModel(eggPath.c_str());
}

PackTheme::~PackTheme()
{
    if (_egg.has_value())
        UnloadModel(*_egg);
}

void PackTheme::drawTile(Vector3 pos, Vector3 size, bool isLight) const
{
    _fallback.drawTile(pos, size, isLight);
}

void PackTheme::drawResource(std::size_t resourceIndex, Vector3 pos, float height) const
{
    _fallback.drawResource(resourceIndex, pos, height);
}

void PackTheme::drawPlayer(Vector3 pos, float rotationDeg) const
{
    if (_player) {
        float frame = std::fmod(GetTime() * ANIM_FPS, _player->getAnimationFrameCount(WALK_ANIM_INDEX));
        _player->draw(pos, rotationDeg, WALK_ANIM_INDEX, frame);
        return;
    }
    _fallback.drawPlayer(pos, rotationDeg);
}

void PackTheme::drawEgg(Vector3 pos) const
{
    if (_egg.has_value()) {
        DrawModel(*_egg, pos, 1.0f, WHITE);
        return;
    }
    _fallback.drawEgg(pos);
}

}
