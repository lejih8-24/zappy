/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Static GLB model wrapper
*/

#pragma once

#include "Graphics/Types.hpp"
#include <memory>
#include <string_view>

namespace GUI {

class StaticModel {
    public:
        explicit StaticModel(std::string_view path);
        ~StaticModel();

        StaticModel(const StaticModel &) = delete;
        StaticModel &operator=(const StaticModel &) = delete;

        void draw(Vec3 position, float scale, Vec3 rotation, Vec3 translation = {}) const;

    private:
        struct ModelData;
        std::unique_ptr<ModelData> _modelData;
};

}
