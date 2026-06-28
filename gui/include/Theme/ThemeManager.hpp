/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Loads and owns the active theme for the current pack
*/

#pragma once

#include "Theme/ITheme.hpp"

#include <memory>
#include <string_view>

namespace GUI {

class ThemeManager {
    public:
        explicit ThemeManager(std::string_view packName = "default");

        ITheme &active();

    private:
        std::unique_ptr<ITheme> _theme;
};

}
