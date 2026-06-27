/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CharacterModel.hpp
*/

#pragma once

#include "Graphics/Types.hpp"
#include <memory>
#include <string_view>

namespace GUI {
class CharacterModel {
    public:
        CharacterModel();
        explicit CharacterModel(std::string_view path, bool loadAnimations = true);
        ~CharacterModel();

        CharacterModel(const CharacterModel &) = delete; // two copies sharing the same GPU handle would double-free on destruction = crash
        CharacterModel &operator=(const CharacterModel &) = delete;

        void draw(Vec3 position, float rotationDeg, int animationIndex, float frame, float scale = 1.0f) const;
        void applyRotation(float xDeg, float yDeg, float zDeg);
        int getAnimationFrameCount(int animationIndex) const;

    private:
        struct ModelData;
        std::unique_ptr<ModelData> _modelData;
};
}
