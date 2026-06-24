/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CharacterModel.cpp
*/

#include "Game/CharacterModel.hpp"
#include <stdexcept>
#include <string>

static constexpr const char *DEFAULT_MODEL_PATH = PACKS_DIR "green_man/player.glb";

GUI::CharacterModel::CharacterModel() : CharacterModel(DEFAULT_MODEL_PATH)
{
}

GUI::CharacterModel::CharacterModel(std::string_view path)
    : _model(LoadModel(std::string(path).c_str()))
    , _animations(LoadModelAnimations(std::string(path).c_str(), &_animationCount))
{
    if (_animationCount <= 0)
        throw std::runtime_error("character model has no animations");
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