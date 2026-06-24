/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EggModel.hpp
*/

#pragma once

#include "raylib.h"

namespace GUI {

class EggModel {
    public:
        EggModel();
        ~EggModel();

        EggModel(const EggModel &) = delete;
        EggModel &operator=(const EggModel &) = delete;

        void draw(Vector3 position) const;

    private:
        Model _model;
};

}