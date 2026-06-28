/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Static GLB model wrapper
*/

#include "Game/StaticModel.hpp"

#include "raylib.h"
#include "raymath.h"

#include <memory>
#include <string>

struct GUI::StaticModel::ModelData {
    Model model;
};

static Matrix rotationMatrix(GUI::Vec3 rotation)
{
    Matrix rx = MatrixRotateX(DEG2RAD * rotation.x);
    Matrix ry = MatrixRotateY(DEG2RAD * rotation.y);
    Matrix rz = MatrixRotateZ(DEG2RAD * rotation.z);

    // raymath has no variadic multiply; two nested calls produce rx * ry * rz.
    return MatrixMultiply(MatrixMultiply(rx, ry), rz);
}

GUI::StaticModel::StaticModel(std::string_view path)
    : _modelData(std::make_unique<ModelData>())
{
    _modelData->model = LoadModel(std::string(path).c_str());
}

GUI::StaticModel::~StaticModel()
{
    UnloadModel(_modelData->model);
}

void GUI::StaticModel::draw(Vec3 position, float scale, Vec3 rotation, Vec3 translation) const
{
    Matrix s = MatrixScale(scale, scale, scale);
    Matrix t = MatrixTranslate(position.x + translation.x, position.y + translation.y, position.z + translation.z);
    Matrix transform = MatrixMultiply(MatrixMultiply(s, rotationMatrix(rotation)), t);

    for (int i = 0; i < _modelData->model.meshCount; i++)
        DrawMesh(_modelData->model.meshes[i], _modelData->model.materials[_modelData->model.meshMaterial[i]], transform);
}
