/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Game Resources struct
*/

#include "Resources.hpp"
#include <zappy/utils.hpp>
#include <sstream>


/**
 * Determines if the specified
 * resource amounts are all present
 * in this resource collection.
 */
bool Zappy::Game::Resources::contains(const Resources& amount) const
{
    for (auto [resourceAmount, requiredAmount] : Utils::zip(*this, amount)) {
        if (resourceAmount < requiredAmount)
            return false;
    }

    return true;
}

/**
 * Adds the resource amounts from the
 * other resources struct to this
 * resource struct.
 */
auto Zappy::Game::Resources::operator+=(const Resources& other) -> Resources&
{
    for (auto [resource, other] : Utils::zip(*this, other)) {
        resource += other;
    }

    return *this;
}

/**
 * Subtracts the resource amounts from the
 * other resources struct to this
 * resource struct.
 */
auto Zappy::Game::Resources::operator-=(const Resources& other) -> Resources&
{
    for (auto [resource, other] : Utils::zip(*this, other)) {
        resource -= other;
    }

    return *this;
}

/**
 * Gets a string representation of
 * the resources.
 */
Zappy::Game::Resources::operator std::string() const
{
    std::ostringstream repr;
    repr << *this;
    return repr.str();
}

/**
 * Displays the available resources
 * in the given output stream.
 */
std::ostream& operator<<(std::ostream& stream, const Zappy::Game::Resources& resources)
{
    return stream << "Resources{ "
                  << ".food = "      << resources.food      << ", "
                  << ".linemate = "  << resources.linemate  << ", "
                  << ".deraumere = " << resources.deraumere << ", "
                  << ".sibur = "     << resources.sibur     << ", "
                  << ".mendiane = "  << resources.mendiane  << ", "
                  << ".phiras = "    << resources.phiras    << ", "
                  << ".thystame = "  << resources.thystame
                  <<" }";
}
