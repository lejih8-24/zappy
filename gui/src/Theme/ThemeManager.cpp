/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Loads and owns the active theme for the current pack
*/

#include "Theme/ThemeManager.hpp"
#include "Theme/DefaultTheme.hpp"
#include "Theme/PackTheme.hpp"

namespace GUI {

ThemeManager::ThemeManager(std::string_view packName)
{
    if (packName == "default" || packName.empty())
        _theme = std::make_unique<DefaultTheme>();
    else
        _theme = std::make_unique<PackTheme>(packName);
}

ITheme &ThemeManager::active()
{
    return *_theme;
}

}
