/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Generic pack theme - missing assets fall back to primitives
*/

#include "Theme/PackTheme.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include "raymath.h"

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

// raylib converts u32 (UNSIGNED_INT) mesh indices to u16, corrupting any index > 65535.
// Calling LoadModel on such a file causes a segfault inside raylib's skin processor.
// We pre-check the GLB JSON chunk for componentType 5125 (UNSIGNED_INT) and refuse to
// load the file if found, falling back to primitives instead of crashing.
static bool glbHasU32Indices(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
        return false;
    uint32_t magic = 0, chunkLen = 0, chunkType = 0;
    file.read(reinterpret_cast<char *>(&magic), 4);
    if (magic != 0x46546C67u) // GLB magic: 'glTF' in little-endian ASCII
        return false;
    file.seekg(12); // skip 12-byte GLB header: magic(4) + version(4) + totalLength(4)
    file.read(reinterpret_cast<char *>(&chunkLen), 4);
    file.read(reinterpret_cast<char *>(&chunkType), 4);
    if (chunkType != 0x4E4F534Au) // chunk type must be 'JSON' in little-endian ASCII
        return false;
    std::string json(chunkLen, '\0');
    file.read(json.data(), chunkLen);
    // 5125 = UNSIGNED_INT in the glTF accessor.componentType spec
    return json.find("\"componentType\":5125") != std::string::npos
        || json.find("\"componentType\": 5125") != std::string::npos;
}

static std::string resolvePath(std::string_view packName, const char *filename)
{
    std::string path = std::string(PACKS_DIR) + std::string(packName) + "/" + filename;
    return std::filesystem::exists(path) ? path : std::string{};
}

static Vector3 parseRotation(std::string_view packName, const char *blockName)
{
    std::string manifestPath = std::string(PACKS_DIR) + std::string(packName) + "/manifest.json";
    if (!std::filesystem::exists(manifestPath))
        return {0, 0, 0};

    std::ifstream file(manifestPath);
    std::ostringstream buf;
    buf << file.rdbuf();
    std::string json = buf.str();

    auto blockStart = json.find(std::string("\"") + blockName + "\"");
    if (blockStart == std::string::npos)
        return {0, 0, 0};
    // clamp search to the {...} block so later fields with the same axis keys don't overwrite
    auto blockOpen = json.find('{', blockStart);
    auto blockClose = json.find('}', blockOpen);
    if (blockOpen == std::string::npos || blockClose == std::string::npos)
        return {0, 0, 0};

    Vector3 rot = {0, 0, 0};
    // matches `"x": 180` or `"z": -90.5`; group 1 = axis letter, group 2 = signed float
    std::regex axis("\"([xyz])\"\\s*:\\s*(-?\\d+(?:\\.\\d+)?)");
    std::sregex_iterator it(json.cbegin() + blockOpen, json.cbegin() + blockClose, axis);
    std::sregex_iterator end;
    for (; it != end; ++it) {
        char a = (*it)[1].str()[0];
        float v = std::stof((*it)[2].str());
        if (a == 'x') rot.x = v;
        else if (a == 'y') rot.y = v;
        else if (a == 'z') rot.z = v;
    }
    return rot;
}

static Color parseBackgroundColor(std::string_view packName)
{
    std::string manifestPath = std::string(PACKS_DIR) + std::string(packName) + "/manifest.json";
    if (!std::filesystem::exists(manifestPath))
        return DARKBLUE;

    std::ifstream file(manifestPath);
    std::ostringstream buf;
    buf << file.rdbuf();
    std::string json = buf.str();

    auto blockStart = json.find("\"backgroundColor\"");
    if (blockStart == std::string::npos)
        return DARKBLUE;
    auto blockOpen = json.find('{', blockStart);
    auto blockClose = json.find('}', blockOpen);
    if (blockOpen == std::string::npos || blockClose == std::string::npos)
        return DARKBLUE;

    Color c = DARKBLUE;
    std::regex channel("\"([rgb])\"\\s*:\\s*(\\d+)");
    std::sregex_iterator it(json.cbegin() + blockOpen, json.cbegin() + blockClose, channel);
    std::sregex_iterator end;
    for (; it != end; ++it) {
        char ch = (*it)[1].str()[0];
        int v = std::clamp(std::stoi((*it)[2].str()), 0, 255);
        if (ch == 'r') c.r = static_cast<unsigned char>(v);
        else if (ch == 'g') c.g = static_cast<unsigned char>(v);
        else if (ch == 'b') c.b = static_cast<unsigned char>(v);
    }
    c.a = 255;
    return c;
}

static Matrix parseRotationMatrix(std::string_view packName, const char *blockName)
{
    Vector3 rot = parseRotation(packName, blockName);
    Matrix rx = MatrixRotateX(DEG2RAD * rot.x);
    Matrix ry = MatrixRotateY(DEG2RAD * rot.y);
    Matrix rz = MatrixRotateZ(DEG2RAD * rot.z);
    return MatrixMultiply(MatrixMultiply(rx, ry), rz);
}

static void drawStaticModel(const Model &model, Vector3 pos, float scale, const Matrix &correction,
    Vector3 offset = {0.0f, 0.0f, 0.0f})
{
    Matrix s = MatrixScale(scale, scale, scale);
    Matrix t = MatrixTranslate(pos.x + offset.x, pos.y + offset.y, pos.z + offset.z);
    Matrix transform = MatrixMultiply(MatrixMultiply(s, correction), t);
    for (int i = 0; i < model.meshCount; i++)
        DrawMesh(model.meshes[i], model.materials[model.meshMaterial[i]], transform);
}

static constexpr std::array<const char *, 7> RESOURCE_NAMES = {
    "food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"
};

static std::size_t findMatchingBrace(const std::string &json, std::size_t openPos)
{
    int depth = 0;
    for (std::size_t i = openPos; i < json.size(); ++i) {
        if (json[i] == '{') ++depth;
        else if (json[i] == '}') { --depth; if (depth == 0) return i; }
    }
    return std::string::npos;
}

static std::string parseResourceBlock(const std::string &json, std::string_view resourceName)
{
    std::string key = std::string("\"") + std::string(resourceName) + "\"";
    auto pos = json.find(key);
    if (pos == std::string::npos)
        return {};
    auto open = json.find('{', pos + key.size());
    if (open == std::string::npos)
        return {};
    auto close = findMatchingBrace(json, open);
    if (close == std::string::npos)
        return {};
    return json.substr(open + 1, close - open - 1);
}

static std::unordered_map<std::string, int> parseAnimations(std::string_view packName)
{
    std::unordered_map<std::string, int> result;

    std::string manifestPath = std::string(PACKS_DIR) + std::string(packName) + "/manifest.json";
    if (!std::filesystem::exists(manifestPath))
        return result;

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

static float parseManifestFloat(std::string_view packName, const char *key, float defaultValue,
    float minValue = -1e30f, float maxValue = 1e30f)
{
    // clamp the default too so a bad default can't bypass the range check
    defaultValue = std::clamp(defaultValue, minValue, maxValue);

    std::string manifestPath = std::string(PACKS_DIR) + std::string(packName) + "/manifest.json";
    if (!std::filesystem::exists(manifestPath))
        return defaultValue;

    std::ifstream file(manifestPath);
    std::ostringstream buf;
    buf << file.rdbuf();
    std::string json = buf.str();

    std::regex r(std::string("\"") + key + "\"\\s*:\\s*(\\d+(?:\\.\\d+)?)");
    std::smatch m;
    if (std::regex_search(json, m, r))
        return std::clamp(std::stof(m[1].str()), minValue, maxValue);
    return defaultValue;
}

static std::array<GUI::PackTheme::ResourceOverride, 7> parseResourceOverrides(std::string_view packName)
{
    std::array<GUI::PackTheme::ResourceOverride, 7> result{};

    std::string manifestPath = std::string(PACKS_DIR) + std::string(packName) + "/manifest.json";
    if (!std::filesystem::exists(manifestPath))
        return result;

    std::ifstream file(manifestPath);
    std::ostringstream buf;
    buf << file.rdbuf();
    std::string json = buf.str();

    std::regex scaleRe("\"scale\"\\s*:\\s*(\\d+(?:\\.\\d+)?)");
    std::regex axisRe("\"([xyz])\"\\s*:\\s*(-?\\d+(?:\\.\\d+)?)");

    for (std::size_t i = 0; i < RESOURCE_NAMES.size(); ++i) {
        std::string block = parseResourceBlock(json, RESOURCE_NAMES[i]);
        if (block.empty())
            continue;

        std::smatch sm;
        if (std::regex_search(block, sm, scaleRe))
            result[i].scale = std::clamp(std::stof(sm[1].str()), 0.000001f, 100.0f);

        auto parseSubBlock = [&](const char *subKey) -> std::string {
            auto pos = block.find(std::string("\"") + subKey + "\"");
            if (pos == std::string::npos) return {};
            auto open = block.find('{', pos);
            if (open == std::string::npos) return {};
            auto close = findMatchingBrace(block, open);
            if (close == std::string::npos) return {};
            return block.substr(open + 1, close - open - 1);
        };

        std::string rotBlock = parseSubBlock("rotation");
        if (!rotBlock.empty()) {
            Vector3 rot = {0, 0, 0};
            std::sregex_iterator it(rotBlock.begin(), rotBlock.end(), axisRe);
            for (; it != std::sregex_iterator{}; ++it) {
                char a = (*it)[1].str()[0];
                float v = std::stof((*it)[2].str());
                if (a == 'x') rot.x = v;
                else if (a == 'y') rot.y = v;
                else if (a == 'z') rot.z = v;
            }
            Matrix rx = MatrixRotateX(DEG2RAD * rot.x);
            Matrix ry = MatrixRotateY(DEG2RAD * rot.y);
            Matrix rz = MatrixRotateZ(DEG2RAD * rot.z);
            result[i].correction = MatrixMultiply(MatrixMultiply(rx, ry), rz);
        }

        std::string transBlock = parseSubBlock("translation");
        if (!transBlock.empty()) {
            Vector3 trans = {0, 0, 0};
            std::sregex_iterator it(transBlock.begin(), transBlock.end(), axisRe);
            for (; it != std::sregex_iterator{}; ++it) {
                char a = (*it)[1].str()[0];
                float v = std::stof((*it)[2].str());
                if (a == 'x') trans.x = v;
                else if (a == 'y') trans.y = v;
                else if (a == 'z') trans.z = v;
            }
            result[i].translation = trans;
        }
    }
    return result;
}

namespace GUI {

PackTheme::PackTheme(std::string_view packName)
    : _animations(parseAnimations(packName))
    , _eggCorrection(MatrixIdentity())
    , _playerScale(parseManifestFloat(packName, "playerScale", 1.0f, 0.001f, 100.0f))
    , _playerLabelHeight(parseManifestFloat(packName, "playerLabelHeight", 2.5f, 0.0f, 20.0f))
    , _playerLabelScale(parseManifestFloat(packName, "playerLabelScale", 140.0f, 10.0f, 500.0f))
    , _backgroundColor(parseBackgroundColor(packName))
{
    std::string packDir = std::string(PACKS_DIR) + std::string(packName);
    if (!std::filesystem::exists(packDir) || !std::filesystem::is_directory(packDir)) {
        std::cerr << "ERROR: Pack '" << packName << "' not found at " << packDir << "\n";
        std::exit(1);
    }
    std::string playerPath = resolvePath(packName, "player.glb");
    if (!playerPath.empty() && !glbHasU32Indices(playerPath)) {
        try {
            _player = std::make_unique<CharacterModel>(playerPath, !_animations.empty());
            Vector3 rot = parseRotation(packName, "playerRotation");
            if (rot.x != 0 || rot.y != 0 || rot.z != 0)
                _player->applyRotation(rot.x, rot.y, rot.z);
            _playerTranslation = parseRotation(packName, "playerTranslation");
        } catch (...) {
            _player = nullptr;
        }
    }

    std::string eggPath = resolvePath(packName, "egg.glb");
    if (!eggPath.empty()) {
        _egg = LoadModel(eggPath.c_str());
        _eggScale = parseManifestFloat(packName, "eggScale", 1.0f, 0.000001f, 100.0f);
        _eggCorrection = parseRotationMatrix(packName, "eggRotation");
        _eggTranslation = parseRotation(packName, "eggTranslation");
    }

    std::string tilePath = resolvePath(packName, "tile.glb");
    if (!tilePath.empty()) {
        _tile = LoadModel(tilePath.c_str());
        _tileScale = parseManifestFloat(packName, "tileScale", 1.0f, 0.000001f, 100.0f);
        _tileCorrection = parseRotationMatrix(packName, "tileRotation");
        _tileTranslation = parseRotation(packName, "tileTranslation");
    }

    _resourceScale = parseManifestFloat(packName, "resourceScale", 1.0f, 0.000001f, 100.0f);
    _resourceCorrection = parseRotationMatrix(packName, "resourceRotation");
    _resourceTranslation = parseRotation(packName, "resourceTranslation");
    _resourceOverrides = parseResourceOverrides(packName);
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
        drawStaticModel(*_tile, pos, _tileScale, _tileCorrection, _tileTranslation);
        return;
    }
    _fallback.drawTile(pos, size, isLight);
}

void PackTheme::drawResource(std::size_t resourceIndex, Vector3 pos) const
{
    if (resourceIndex < _resources.size() && _resources[resourceIndex].has_value()) {
        const auto &ov = _resourceOverrides[resourceIndex];
        float scale = ov.scale.value_or(_resourceScale);
        Matrix corr = ov.correction.value_or(_resourceCorrection);
        Vector3 trans = ov.translation.value_or(_resourceTranslation);
        drawStaticModel(*_resources[resourceIndex], pos, scale, corr, trans);
        return;
    }
    _fallback.drawResource(resourceIndex, pos);
}

void PackTheme::drawPlayer(Vector3 pos, float rotationDeg, Player::AnimState state,
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
        Vector3 translatedPos = { pos.x + _playerTranslation.x, pos.y + _playerTranslation.y, pos.z + _playerTranslation.z };
        _player->draw(translatedPos, rotationDeg, animIdx, frame, _playerScale);
        return;
    }
    _fallback.drawPlayer(pos, rotationDeg, state, animationElapsed);
}

void PackTheme::drawEgg(Vector3 pos) const
{
    if (_egg.has_value()) {
        drawStaticModel(*_egg, pos, _eggScale, _eggCorrection, _eggTranslation);
        return;
    }
    _fallback.drawEgg(pos);
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
