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
        void drawPlayer(Vector3 pos, float rotationDeg) const override;
        void drawEgg(Vector3 pos) const override;

        int getAnimIndex(const std::string &name, int defaultIndex = 0) const;

    private:
        std::unordered_map<std::string, int> _animations;
        DefaultTheme _fallback;
        std::unique_ptr<CharacterModel> _player;
        std::optional<Model> _egg;
};

}
