/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CharacterModel.hpp
*/

#pragma once

#include "raylib.h"
#include <string_view>

namespace GUI {
class CharacterModel {
    public:
        CharacterModel();
        explicit CharacterModel(std::string_view path, bool loadAnimations = true);
        ~CharacterModel();

        CharacterModel(const CharacterModel &) = delete; // on interdit copie de l'objet sinon 2 copies pointent vers meme mémoire gpu/ram allouée par raylib et destructeur libère 2 fois = crash
        CharacterModel &operator=(const CharacterModel &) = delete;

        void draw(Vector3 position, float rotationDeg, int animationIndex, float frame) const;
        void applyRotation(float xDeg, float yDeg, float zDeg);
        int getAnimationFrameCount(int animationIndex) const;

    private:
        Model _model;
        ModelAnimation *_animations;
        int _animationCount;
        Matrix _correction;
};
}