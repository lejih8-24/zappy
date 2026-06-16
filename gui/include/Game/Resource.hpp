/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI resources storage types
*/

#pragma once

#include <array>
#include <cstddef>

namespace GUI {

enum class Resource {
    Food,
    Linemate,
    Deraumere,
    Sibur,
    Mendiane,
    Phiras,
    Thystame,
    Resource_Count, //? KEEP IN LAST ALWAYS -> used to determine nb of enum elem
};

inline constexpr std::size_t ResourceCount = static_cast<std::size_t>(Resource::Resource_Count);
using ResourceStock = std::array<int, ResourceCount>;

}
