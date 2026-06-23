/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CharacterModel.cpp
*/

#include "Game/CharacterModel.hpp"
#include <stdexcept>

static constexpr const char *MODEL_PATH = ASSETS_DIR "character.glb";

GUI::CharacterModel::CharacterModel() : _model(LoadModel(MODEL_PATH)), _animations(LoadModelAnimations(MODEL_PATH, &_animationCount))
{
    if (_animationCount <= 0) {
        throw std::runtime_error("character model has no animations");
    }
}

GUI::CharacterModel::~CharacterModel()
{
    UnloadModelAnimations(_animations, _animationCount);
    UnloadModel(_model);
}

void GUI::CharacterModel::draw(Vector3 position, float rotationDeg, int animationIndex, float frame) const
{
    UpdateModelAnimation(_model, _animations[animationIndex], frame);
    DrawModelEx(_model, position, {0.0f, 1.0f, 0.0f}, rotationDeg, {1.0f, 1.0f, 1.0f}, WHITE);
}

int GUI::CharacterModel::getAnimationFrameCount(int animationIndex) const
{
    return _animations[animationIndex].keyframeCount;
}