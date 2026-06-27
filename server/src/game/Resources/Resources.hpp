/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Game Resources struct
*/

#pragma once

#include <ostream>
#include <string>
#include <optional>


namespace Zappy::Game {
    enum class ResourceType {
        FOOD,
        LINEMATE,
        DERAUMERE,
        SIBUR,
        MENDIANE,
        PHIRAS,
        THYSTAME,
    };

    /**
     * Zappy game resources.
     */
    struct Resources {
        using Quantity = unsigned int;
        using Iterator = Quantity*;
        using ConstIterator = const Quantity*;

        static inline constexpr std::size_t RESOURCE_COUNT = 7;

        Quantity food;
        Quantity linemate;
        Quantity deraumere;
        Quantity sibur;
        Quantity mendiane;
        Quantity phiras;
        Quantity thystame;

        bool contains(const Resources& amount) const;
        bool empty() const;

        inline Resources operator+(const Resources& other) const { return Resources(*this) += other; }
        Resources& operator+=(const Resources& other);

        inline Resources operator-(const Resources& other) const { return Resources(*this) -= other; }
        Resources& operator-=(const Resources& other);

        inline Quantity& operator[](ResourceType type) noexcept { return reinterpret_cast<Quantity*>(this)[static_cast<std::size_t>(type)]; }
        inline const Quantity& operator[](ResourceType type) const noexcept { return reinterpret_cast<const Quantity*>(this)[static_cast<std::size_t>(type)]; }

        operator std::string() const;

        inline Iterator begin() { return reinterpret_cast<Quantity*>(this); }
        inline Iterator end() { return reinterpret_cast<Quantity*>(this) + RESOURCE_COUNT; }

        inline ConstIterator begin() const { return reinterpret_cast<const Quantity*>(this); }
        inline ConstIterator end() const { return reinterpret_cast<const Quantity*>(this) + RESOURCE_COUNT; }

        inline ConstIterator cbegin() const { return begin(); }
        inline ConstIterator cend() const { return end(); }
    };

    std::optional<ResourceType> fromString(std::string_view repr);
}

std::ostream& operator<<(std::ostream& stream, const Zappy::Game::Resources& resources);
