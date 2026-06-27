/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CharacterModel.cpp
*/

#include "Game/CharacterModel.hpp"

#include "raylib.h"
#include "raymath.h"

#include <memory>
#include <stdexcept>
#include <string>

static constexpr const char *DEFAULT_MODEL_PATH = PACKS_DIR "penguin/player.glb";

struct GUI::CharacterModel::ModelData {
    Model model;
    ModelAnimation *animations = nullptr;
    int animationCount = 0;
    Matrix correction = MatrixIdentity();
};

GUI::CharacterModel::CharacterModel() : CharacterModel(DEFAULT_MODEL_PATH)
{
}

GUI::CharacterModel::CharacterModel(std::string_view path, bool loadAnimations)
    : _modelData(std::make_unique<ModelData>())
{
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
