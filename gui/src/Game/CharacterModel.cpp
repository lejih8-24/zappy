/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CharacterModel.cpp
*/

#include "Game/CharacterModel.hpp"
#include <stdexcept>
#include <string>
#include "raymath.h"

static constexpr const char *DEFAULT_MODEL_PATH = PACKS_DIR "green_man/player.glb";

GUI::CharacterModel::CharacterModel() : CharacterModel(DEFAULT_MODEL_PATH)
{
}

GUI::CharacterModel::CharacterModel(std::string_view path, bool loadAnimations)
    : _model(LoadModel(std::string(path).c_str()))
    , _animations(nullptr)
    , _correction(MatrixIdentity())
{
    // raylib stores indices as u16 internally; a mesh exceeding 65535 vertices would
    // overflow them silently. UnloadModel first: LoadModel already uploaded VAOs to the GPU.
    for (int i = 0; i < _model.meshCount; i++) {
        if (_model.meshes[i].vertexCount > 65535) {
            UnloadModel(_model);
            throw std::runtime_error("model exceeds raylib u16 index limit - reduce poly count");
        }
    }
    // skip LoadModelAnimations for static models (no animations block in manifest);
    // calling it on a file with no animation data causes a crash inside raylib
    if (loadAnimations)
        _animations = LoadModelAnimations(std::string(path).c_str(), &_animationCount);
}

GUI::CharacterModel::~CharacterModel()
{
    if (_animations)
        UnloadModelAnimations(_animations, _animationCount);
    UnloadModel(_model);
}

void GUI::CharacterModel::draw(Vector3 position, float rotationDeg, int animationIndex, float frame, float scale) const
{
    if (_animationCount > 0)
        // % guard: prevents OOB if caller passes a stale or out-of-range animation index
        UpdateModelAnimation(_model, _animations[animationIndex % _animationCount], frame);
    Matrix s = MatrixScale(scale, scale, scale);
    Matrix yaw = MatrixRotate({0.0f, 1.0f, 0.0f}, rotationDeg * DEG2RAD);
    Matrix translation = MatrixTranslate(position.x, position.y, position.z);
    // DrawModelEx composes its own transform on top of model.transform, so setting
    // model.transform to a correction matrix applies it in the wrong space for animated
    // meshes. DrawMesh with an explicit matrix gives full control over the order.
    // Column-vector convention (rightmost applied first): scale(model space) * correction * yaw * translation(world).
    Matrix transform = MatrixMultiply(MatrixMultiply(MatrixMultiply(s, _correction), yaw), translation);
    for (int i = 0; i < _model.meshCount; i++)
        DrawMesh(_model.meshes[i], _model.materials[_model.meshMaterial[i]], transform);
}

void GUI::CharacterModel::applyRotation(float xDeg, float yDeg, float zDeg)
{
    Matrix rx = MatrixRotateX(DEG2RAD * xDeg);
    Matrix ry = MatrixRotateY(DEG2RAD * yDeg);
    Matrix rz = MatrixRotateZ(DEG2RAD * zDeg);
    // raymath has no variadic multiply; two nested calls produce rx * ry * rz
    _correction = MatrixMultiply(MatrixMultiply(rx, ry), rz);
}

int GUI::CharacterModel::getAnimationFrameCount(int animationIndex) const
{
    return _animations[animationIndex].keyframeCount;
}