/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Generic pack theme with three-level asset fallback chain
*/

#pragma once

#include "Theme/ITheme.hpp"
#include "Theme/DefaultTheme.hpp"
#include "Theme/PackManifest.hpp"
#include "Game/CharacterModel.hpp"
#include "Game/StaticModel.hpp"

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

        void drawTile(const ICanvas &canvas, Vec3 pos, Vec3 size, bool isLight) const override;
        void drawResource(const ICanvas &canvas, std::size_t resourceIndex, Vec3 pos) const override;
        void drawPlayer(const ICanvas &canvas, Vec3 pos, float rotationDeg, Player::AnimState state,
            float animationElapsed = 0.0F) const override;
        void drawEgg(const ICanvas &canvas, Vec3 pos) const override;

        int getAnimIndex(const std::string &name, int defaultIndex = 0) const;

        float getPlayerLabelHeight() const override;
        float getPlayerLabelScale() const override;
        float getPlayerAnimationDuration(Player::AnimState state) const override;
        Color getBackgroundColor() const override;

    private:
        std::unordered_map<std::string, int> _animations;
        DefaultTheme _fallback;
        std::unique_ptr<CharacterModel> _player;
        std::optional<StaticModel> _egg;
        float _eggScale = 1.0f;
        Vec3 _eggRotation = {};
        Vec3 _eggTranslation = {};
        std::optional<StaticModel> _tile;
        float _tileScale = 1.0f;
        Vec3 _tileRotation = {};
        Vec3 _tileTranslation = {};
        std::array<std::optional<StaticModel>, 7> _resources;
        float _resourceScale = 1.0f;
        Vec3 _resourceRotation = {};
        Vec3 _resourceTranslation = {};
        std::array<PackManifest::ResourceOverride, 7> _resourceOverrides{};

        float _playerScale = 1.0f;
        float _playerLabelHeight = 2.5f;
        float _playerLabelScale = 140.0f;
        Vec3 _playerTranslation = {};
        Color _backgroundColor = Colors::DarkBlue;
};

}
