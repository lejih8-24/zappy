/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Generic pack theme with three-level asset fallback chain
*/

#pragma once

#include "Theme/ITheme.hpp"
#include "Theme/DefaultTheme.hpp"
#include "Game/CharacterModel.hpp"

#include "raylib.h"

#include <array>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace GUI {

class PackTheme : public ITheme {
    public:
        explicit PackTheme(std::string_view packName);
        ~PackTheme();

        void drawTile(Vector3 pos, Vector3 size, bool isLight) const override;
        void drawResource(std::size_t resourceIndex, Vector3 pos, float height) const override;
        void drawPlayer(Vector3 pos, float rotationDeg, Player::AnimState state) const override;
        void drawEgg(Vector3 pos) const override;

        int getAnimIndex(const std::string &name, int defaultIndex = 0) const;

        float getPlayerLabelHeight() const override;
        float getPlayerLabelScale() const override;
        Color getBackgroundColor() const override;

    private:
        std::unordered_map<std::string, int> _animations;
        DefaultTheme _fallback;
        std::unique_ptr<CharacterModel> _player;
        std::optional<Model> _egg;
        float _eggScale = 1.0f;
        Matrix _eggCorrection{};
        Vector3 _eggTranslation = {0.0f, 0.0f, 0.0f};
        std::optional<Model> _tile;
        float _tileScale = 1.0f;
        Matrix _tileCorrection{};
        Vector3 _tileTranslation = {0.0f, 0.0f, 0.0f};
        std::array<std::optional<Model>, 7> _resources;
        float _resourceScale = 1.0f;
        Matrix _resourceCorrection{};
        Vector3 _resourceTranslation = {0.0f, 0.0f, 0.0f};

        float _playerScale = 1.0f;
        float _playerLabelHeight = 2.5f;
        float _playerLabelScale = 140.0f;
        Vector3 _playerTranslation = {0.0f, 0.0f, 0.0f};
        Color _backgroundColor = DARKBLUE;
};

}
