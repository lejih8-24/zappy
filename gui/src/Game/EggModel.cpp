/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EggModel.cpp
*/

#include "Game/EggModel.hpp"

static constexpr const char *MODEL_PATH = PACKS_DIR "green_man/egg.glb";

GUI::EggModel::EggModel() : _model(LoadModel(MODEL_PATH))
{

}

GUI::EggModel::~EggModel()
{
    UnloadModel(_model);
}

void GUI::EggModel::draw(Vector3 position) const
{
    DrawModelEx(_model, position, {1.0f, 0.0f, 0.0f}, -90.0f, {0.03f, 0.03f, 0.03f}, WHITE);
}