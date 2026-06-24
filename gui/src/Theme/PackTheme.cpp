/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Generic pack theme with three-level asset fallback chain
*/

#include "Theme/PackTheme.hpp"

#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

static constexpr float ANIM_FPS = 24.0f;
static constexpr const char *BASE_PACK = "green_man";

static constexpr std::array<const char *, 7> RESOURCE_FILES = {
    "food.glb", "linemate.glb", "deraumere.glb", "sibur.glb",
    "mendiane.glb", "phiras.glb", "thystame.glb"
};

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

static std::unordered_map<std::string, int> parseAnimations(std::string_view packName)
{
    std::unordered_map<std::string, int> result;

    std::string manifestPath = std::string(PACKS_DIR) + std::string(packName) + "/manifest.json";
    if (!std::filesystem::exists(manifestPath)) {
        manifestPath = std::string(PACKS_DIR) + BASE_PACK + "/manifest.json";
        if (!std::filesystem::exists(manifestPath))
            return result;
    }

    std::ifstream file(manifestPath);
    std::ostringstream buf;
    buf << file.rdbuf();
    std::string json = buf.str();

    auto animStart = json.find("\"animations\"");
    if (animStart == std::string::npos)
        return result;
    auto blockStart = json.find('{', animStart);
    auto blockEnd = json.find('}', blockStart);
    if (blockStart == std::string::npos || blockEnd == std::string::npos)
        return result;

    std::string block = json.substr(blockStart + 1, blockEnd - blockStart - 1);
    std::regex entry("\"([^\"]+)\"\\s*:\\s*(\\d+)");
    std::sregex_iterator it(block.begin(), block.end(), entry);
    std::sregex_iterator end;
    for (; it != end; ++it)
        result[(*it)[1].str()] = std::stoi((*it)[2].str());

    return result;
}

namespace GUI {

PackTheme::PackTheme(std::string_view packName)
    : _animations(parseAnimations(packName))
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

    std::string tilePath = resolvePath(packName, "tile.glb");
    if (!tilePath.empty())
        _tile = LoadModel(tilePath.c_str());

    for (std::size_t i = 0; i < RESOURCE_FILES.size(); ++i) {
        std::string rPath = resolvePath(packName, RESOURCE_FILES[i]);
        if (!rPath.empty())
            _resources[i] = LoadModel(rPath.c_str());
    }
}

PackTheme::~PackTheme()
{
    if (_egg.has_value())
        UnloadModel(*_egg);
    if (_tile.has_value())
        UnloadModel(*_tile);
    for (auto &r : _resources)
        if (r.has_value())
            UnloadModel(*r);
}

int PackTheme::getAnimIndex(const std::string &name, int defaultIndex) const
{
    auto it = _animations.find(name);
    return it != _animations.end() ? it->second : defaultIndex;
}

void PackTheme::drawTile(Vector3 pos, Vector3 size, bool isLight) const
{
    if (_tile.has_value()) {
        DrawModel(*_tile, pos, 1.0f, WHITE);
        return;
    }
    _fallback.drawTile(pos, size, isLight);
}

void PackTheme::drawResource(std::size_t resourceIndex, Vector3 pos, float height) const
{
    if (resourceIndex < _resources.size() && _resources[resourceIndex].has_value()) {
        DrawModel(*_resources[resourceIndex], pos, 1.0f, WHITE);
        return;
    }
    _fallback.drawResource(resourceIndex, pos, height);
}

void PackTheme::drawPlayer(Vector3 pos, float rotationDeg) const
{
    if (_player) {
        int walkIdx = getAnimIndex("walk", 0);
        float frame = std::fmod(GetTime() * ANIM_FPS, _player->getAnimationFrameCount(walkIdx));
        _player->draw(pos, rotationDeg, walkIdx, frame);
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
