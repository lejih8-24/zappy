/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CharacterModel.cpp
*/

#include "Game/CharacterModel.hpp"

#include "raylib.h"
#include "raymath.h"

#include <cstdint>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

// The model backend converts u32 (UNSIGNED_INT) mesh indices to u16, corrupting any index > 65535.
// Loading such a file causes a segfault inside the skin processor.
// We pre-check the GLB JSON chunk for componentType 5125 (UNSIGNED_INT) and refuse to
// load the file if found, so callers can fall back to primitives instead of crashing.
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

struct GUI::CharacterModel::ModelData {
    Model model;
    ModelAnimation *animations = nullptr;
    int animationCount = 0;
    Matrix correction = MatrixIdentity();
};

GUI::CharacterModel::CharacterModel(std::string_view path, bool loadAnimations)
    : _modelData(std::make_unique<ModelData>())
{
    if (glbHasU32Indices(std::string(path)))
        throw std::runtime_error("model uses u32 mesh indices - unsupported by the skinning backend");
    _modelData->model = LoadModel(std::string(path).c_str());
    for (int i = 0; i < _modelData->model.meshCount; i++) {
        // raylib stores indices as u16 internally; a mesh exceeding 65535 vertices would
        // overflow them silently. UnloadModel first: LoadModel already uploaded VAOs to the GPU.
        if (_modelData->model.meshes[i].vertexCount > 65535) {
            UnloadModel(_modelData->model);
            throw std::runtime_error("model exceeds raylib u16 index limit - reduce poly count");
        }
    }
    // Skip LoadModelAnimations for static models (no animations block in manifest);
    // calling it on a file with no animation data causes a crash inside raylib.
    if (loadAnimations)
        _modelData->animations = LoadModelAnimations(std::string(path).c_str(), &_modelData->animationCount);
}

GUI::CharacterModel::~CharacterModel()
{
    if (_modelData->animations)
        UnloadModelAnimations(_modelData->animations, _modelData->animationCount);
    UnloadModel(_modelData->model);
}

void GUI::CharacterModel::draw(Vec3 position, float rotationDeg, int animationIndex, float frame, float scale) const
{
    if (_modelData->animationCount > 0 && animationIndex >= 0)
        UpdateModelAnimation(_modelData->model, _modelData->animations[animationIndex % _modelData->animationCount], frame);
    // DrawModelEx composes its own transform on top of model.transform, so setting
    // model.transform to a correction matrix applies it in the wrong space for animated
    // meshes. DrawMesh with an explicit matrix gives full control over the order.
    Matrix s = MatrixScale(scale, scale, scale);
    Matrix yaw = MatrixRotate({0.0f, 1.0f, 0.0f}, rotationDeg * DEG2RAD);
    Matrix translation = MatrixTranslate(position.x, position.y, position.z);
    // Column-vector convention (rightmost applied first):
    // scale(model space) * correction * yaw * translation(world).
    Matrix transform = MatrixMultiply(MatrixMultiply(MatrixMultiply(s, _modelData->correction), yaw), translation);
    for (int i = 0; i < _modelData->model.meshCount; i++)
        DrawMesh(_modelData->model.meshes[i], _modelData->model.materials[_modelData->model.meshMaterial[i]], transform);
}

void GUI::CharacterModel::applyRotation(float xDeg, float yDeg, float zDeg)
{
    Matrix rx = MatrixRotateX(DEG2RAD * xDeg);
    Matrix ry = MatrixRotateY(DEG2RAD * yDeg);
    Matrix rz = MatrixRotateZ(DEG2RAD * zDeg);

    // raymath has no variadic multiply; two nested calls produce rx * ry * rz.
    _modelData->correction = MatrixMultiply(MatrixMultiply(rx, ry), rz);
}

int GUI::CharacterModel::getAnimationFrameCount(int animationIndex) const
{
    if (!_modelData->animations || animationIndex < 0 || animationIndex >= _modelData->animationCount)
        return 1;
    return _modelData->animations[animationIndex].keyframeCount;
}
