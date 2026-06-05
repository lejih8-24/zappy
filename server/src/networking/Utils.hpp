/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Networking utils
*/

#pragma once

#include <type_traits>


namespace Zappy::Networking {
    template <typename T>
    concept Serializable =
        std::is_standard_layout_v<T> &&
        std::is_trivial_v<T>;
}
