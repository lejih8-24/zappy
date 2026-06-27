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
 * Determines if there are any
 * resources or if everything
 * is empty.
 */
bool Zappy::Game::Resources::empty() const
{
    for (auto resourceAmount : *this) {
        if (resourceAmount != 0)
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
 * Returns a resource type based
 * on a provided string.
 *
 * Note:
 * This assumes the string is all
 * lowercase.
 */
auto Zappy::Game::fromString(std::string_view repr) -> std::optional<ResourceType>
{
    if (repr.empty())
        return std::nullopt;

    switch (repr[0]) {
        case 'f': return repr == "food"      ? std::make_optional(ResourceType::FOOD)      : std::nullopt;
        case 'l': return repr == "linemate"  ? std::make_optional(ResourceType::LINEMATE)  : std::nullopt;
        case 'd': return repr == "deraumere" ? std::make_optional(ResourceType::DERAUMERE) : std::nullopt;
        case 's': return repr == "sibur"     ? std::make_optional(ResourceType::SIBUR)     : std::nullopt;
        case 'm': return repr == "mendiane"  ? std::make_optional(ResourceType::MENDIANE)  : std::nullopt;
        case 'p': return repr == "phiras"    ? std::make_optional(ResourceType::PHIRAS)    : std::nullopt;
        case 't': return repr == "thystame"  ? std::make_optional(ResourceType::THYSTAME)  : std::nullopt;
        default: return std::nullopt;
    }
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
