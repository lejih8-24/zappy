/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CharacterModel.hpp
*/

#pragma once

#include "raylib.h"

namespace GUI {
class CharacterModel {
    public:
        CharacterModel();
        ~CharacterModel();

        CharacterModel(const CharacterModel &) = delete; // on interdit copie de l'objet sinon 2 copies pointent vers meme mémoire gpu/ram allouée par raylib et destructeur libère 2 fois = crash
        CharacterModel &operator=(const CharacterModel &) = delete;

        void draw(Vector3 position, float rotationDeg, int animationIndex, float frame) const; //animationIndex 0 = marche, 1 = mort etc

        int getAnimationFrameCount(int animationIndex) const;

    private:
        Model _model;
        ModelAnimation *_animations;
        int _animationCount;
};
}