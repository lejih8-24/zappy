/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Generic pack theme - missing assets fall back to primitives
*/

#include "Theme/PackTheme.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

static constexpr float ANIM_FPS = 24.0f;

static constexpr std::array<const char *, 7> RESOURCE_FILES = {
    "food.glb", "linemate.glb", "deraumere.glb", "sibur.glb",
    "mendiane.glb", "phiras.glb", "thystame.glb"
};

static const char *getPlayerAnimName(GUI::Player::AnimState state)
{
    switch (state) {
        case GUI::Player::AnimState::Idle:        return "idle";
        case GUI::Player::AnimState::Walk:        return "walk";
        case GUI::Player::AnimState::Incantation: return "incantation";
        case GUI::Player::AnimState::Dead:        return "dead";
        case GUI::Player::AnimState::Broadcast:   return "broadcast";
        case GUI::Player::AnimState::LayingEgg:   return "laying_egg";
        case GUI::Player::AnimState::LevelUp:     return "level_up";
        case GUI::Player::AnimState::Eject:       return "eject";
    }
    return "idle";
}

static std::string resolvePath(std::string_view packName, const char *filename)
{
    std::string path = std::string(PACKS_DIR) + std::string(packName) + "/" + filename;
    return std::filesystem::exists(path) ? path : std::string{};
}

namespace GUI {

PackTheme::PackTheme(std::string_view packName)
{
    std::string packDir = std::string(PACKS_DIR) + std::string(packName);
    if (!std::filesystem::exists(packDir) || !std::filesystem::is_directory(packDir)) {
        std::cerr << "ERROR: Pack '" << packName << "' not found at " << packDir << "\n";
        std::exit(1);
    }

    const PackManifest manifest(packName);
    _animations = manifest.animations();
    _playerScale = manifest.number("playerScale", 1.0f, 0.001f, 100.0f);
    _playerLabelHeight = manifest.number("playerLabelHeight", 2.5f, 0.0f, 20.0f);
    _playerLabelScale = manifest.number("playerLabelScale", 140.0f, 10.0f, 500.0f);
    _backgroundColor = manifest.backgroundColor();

    std::string playerPath = resolvePath(packName, "player.glb");
    if (!playerPath.empty()) {
        // CharacterModel throws on GLB files the skinning backend can't load (u32 indices),
        // so a bad/unsupported player model just falls through to the primitive fallback.
        try {
            _player = std::make_unique<CharacterModel>(playerPath, !_animations.empty());
            Vec3 rot = manifest.rotation("playerRotation");
            if (rot.x != 0 || rot.y != 0 || rot.z != 0)
                _player->applyRotation(rot.x, rot.y, rot.z);
            _playerTranslation = manifest.rotation("playerTranslation");
        } catch (...) {
            _player = nullptr;
        }
    }

    std::string eggPath = resolvePath(packName, "egg.glb");
    if (!eggPath.empty()) {
        _egg.emplace(eggPath);
        _eggScale = manifest.number("eggScale", 1.0f, 0.000001f, 100.0f);
        _eggRotation = manifest.rotation("eggRotation");
        _eggTranslation = manifest.rotation("eggTranslation");
    }

    std::string tilePath = resolvePath(packName, "tile.glb");
    if (!tilePath.empty()) {
        _tile.emplace(tilePath);
        _tileScale = manifest.number("tileScale", 1.0f, 0.000001f, 100.0f);
        _tileRotation = manifest.rotation("tileRotation");
        _tileTranslation = manifest.rotation("tileTranslation");
    }

    _resourceScale = manifest.number("resourceScale", 1.0f, 0.000001f, 100.0f);
    _resourceRotation = manifest.rotation("resourceRotation");
    _resourceTranslation = manifest.rotation("resourceTranslation");
    _resourceOverrides = manifest.resourceOverrides();
    for (std::size_t i = 0; i < RESOURCE_FILES.size(); ++i) {
        std::string rPath = resolvePath(packName, RESOURCE_FILES[i]);
        if (!rPath.empty())
            _resources[i].emplace(rPath);
    }
}

PackTheme::~PackTheme()
{
}

int PackTheme::getAnimIndex(const std::string &name, int defaultIndex) const
{
    auto it = _animations.find(name);
    return it != _animations.end() ? it->second : defaultIndex;
}

void PackTheme::drawTile(const ICanvas &canvas, Vec3 pos, Vec3 size, bool isLight) const
{
    if (_tile.has_value()) {
        _tile->draw(pos, _tileScale, _tileRotation, _tileTranslation);
        return;
    }
    _fallback.drawTile(canvas, pos, size, isLight);
}

void PackTheme::drawResource(const ICanvas &canvas, std::size_t resourceIndex, Vec3 pos) const
{
    if (resourceIndex < _resources.size() && _resources[resourceIndex].has_value()) {
        const auto &ov = _resourceOverrides[resourceIndex];
        float scale = ov.scale.value_or(_resourceScale);
        Vec3 rotation = ov.rotation.value_or(_resourceRotation);
        Vec3 trans = ov.translation.value_or(_resourceTranslation);
        _resources[resourceIndex]->draw(pos, scale, rotation, trans);
        return;
    }
    _fallback.drawResource(canvas, resourceIndex, pos);
}

void PackTheme::drawPlayer(const ICanvas &canvas, Vec3 pos, float rotationDeg, Player::AnimState state,
    float animationElapsed) const
{
    if (_player) {
        int walkIdx = getAnimIndex("walk", 0);
        int animIdx = (state == Player::AnimState::Idle)
            ? walkIdx
            : getAnimIndex(getPlayerAnimName(state), walkIdx);
        int frameCount = _player->getAnimationFrameCount(animIdx);
        float frame = 0.0F;

        if (state != Player::AnimState::Idle && frameCount > 0) {
            frame = animationElapsed * ANIM_FPS;
            if (state == Player::AnimState::Dead)
                frame = std::min(frame, static_cast<float>(frameCount - 1));
            else
                frame = std::fmod(frame, static_cast<float>(frameCount));
        }
        Vec3 translatedPos = { pos.x + _playerTranslation.x, pos.y + _playerTranslation.y, pos.z + _playerTranslation.z };
        _player->draw(translatedPos, rotationDeg, animIdx, frame, _playerScale);
        return;
    }
    _fallback.drawPlayer(canvas, pos, rotationDeg, state, animationElapsed);
}

void PackTheme::drawEgg(const ICanvas &canvas, Vec3 pos) const
{
    if (_egg.has_value()) {
        _egg->draw(pos, _eggScale, _eggRotation, _eggTranslation);
        return;
    }
    _fallback.drawEgg(canvas, pos);
}

float PackTheme::getPlayerLabelHeight() const
{
    return _playerLabelHeight;
}

float PackTheme::getPlayerLabelScale() const
{
    return _playerLabelScale;
}

float PackTheme::getPlayerAnimationDuration(Player::AnimState state) const
{
    if (!_player)
        return _fallback.getPlayerAnimationDuration(state);

    int walkIdx = getAnimIndex("walk", 0);
    int animIdx = (state == Player::AnimState::Idle)
        ? walkIdx
        : getAnimIndex(getPlayerAnimName(state), walkIdx);
    return static_cast<float>(_player->getAnimationFrameCount(animIdx)) / ANIM_FPS;
}

Color PackTheme::getBackgroundColor() const
{
    return _backgroundColor;
}

}
