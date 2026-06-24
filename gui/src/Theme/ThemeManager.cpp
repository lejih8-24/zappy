/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Loads and owns the active theme for the current pack
*/

#include "Theme/ThemeManager.hpp"
#include "Theme/DefaultTheme.hpp"
#include "Theme/GlbTheme.hpp"

#include <iostream>

namespace GUI {

ThemeManager::ThemeManager(std::string_view packName)
{
    if (packName == "glb") {
        _theme = std::make_unique<GlbTheme>();
        return;
    }
    if (packName == "default" || packName.empty()) {
        _theme = std::make_unique<DefaultTheme>();
        return;
    }
    std::cerr << "[theme] pack '" << packName << "' not found, falling back to default\n";
    _theme = std::make_unique<DefaultTheme>();
}

ITheme &ThemeManager::active()
{
    return *_theme;
}

}
