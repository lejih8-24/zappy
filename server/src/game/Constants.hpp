/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Game constants
*/

#pragma once

#include "Resources/Resources.hpp"
#include <chrono>


namespace Zappy::Game {
    inline constexpr unsigned char MAX_PLAYER_LEVEL = 8;
    inline constexpr Resources::Quantity INITIAL_PLAYER_FOOD = 10;

    inline constexpr std::chrono::duration<double, std::milli> FOOD_ALIVE_TIME { 126'000 };  // 126 sec
    inline constexpr std::chrono::duration<double, std::milli> RESOURCE_RESPAWN_DELAY { 20'000 };


    /**
     * Trantor resource densities.
     */
    inline constexpr std::array<float, Zappy::Game::Resources::RESOURCE_COUNT> RESOURCE_DENSITIES = {{
        /* [FOOD]      = */ 0.50f,
        /* [LINEMATE]  = */ 0.30f,
        /* [DERAUMERE] = */ 0.15f,
        /* [SIBUR]     = */ 0.10f,
        /* [MENDIANE]  = */ 0.10f,
        /* [PHIRAS]    = */ 0.08f,
        /* [THYSTAME]  = */ 0.05f,
    }};

    struct IncantationRequirements {
        std::size_t players;
        Resources resources;
    };

    /**
     * Player level up incantation
     * ritual requirements.
     */
    inline constexpr std::array<IncantationRequirements, MAX_PLAYER_LEVEL> INCANTATION_REQUIREMENTS = {{
        { 0 },
        { .players = 1, .resources = { .food = 0, .linemate = 1, .deraumere = 0, .sibur = 0, .mendiane = 0, .phiras = 0, .thystame = 0 }},
        { .players = 2, .resources = { .food = 0, .linemate = 1, .deraumere = 1, .sibur = 1, .mendiane = 0, .phiras = 0, .thystame = 0 }},
        { .players = 2, .resources = { .food = 0, .linemate = 2, .deraumere = 0, .sibur = 1, .mendiane = 0, .phiras = 0, .thystame = 0 }},
        { .players = 4, .resources = { .food = 0, .linemate = 1, .deraumere = 1, .sibur = 2, .mendiane = 0, .phiras = 1, .thystame = 0 }},
        { .players = 4, .resources = { .food = 0, .linemate = 1, .deraumere = 2, .sibur = 1, .mendiane = 3, .phiras = 0, .thystame = 0 }},
        { .players = 6, .resources = { .food = 0, .linemate = 1, .deraumere = 2, .sibur = 3, .mendiane = 0, .phiras = 1, .thystame = 0 }},
        { .players = 6, .resources = { .food = 0, .linemate = 2, .deraumere = 2, .sibur = 2, .mendiane = 2, .phiras = 2, .thystame = 1 }},
    }};

    /**
     * Player Cooldowns
     */
    inline constexpr std::chrono::duration<double, std::milli> PLAYER_FORWARD_COOLDOWN     { 7'000 };
    inline constexpr std::chrono::duration<double, std::milli> PLAYER_RIGHT_COOLDOWN       { 7'000 };
    inline constexpr std::chrono::duration<double, std::milli> PLAYER_LEFT_COOLDOWN        { 7'000 };
    inline constexpr std::chrono::duration<double, std::milli> PLAYER_LOOK_COOLDOWN        { 7'000 };
    inline constexpr std::chrono::duration<double, std::milli> PLAYER_INVENTORY_COOLDOWN   { 1'000 };
    inline constexpr std::chrono::duration<double, std::milli> PLAYER_BROADCAST_COOLDOWN   { 7'000 };
    inline constexpr std::chrono::duration<double, std::milli> PLAYER_FORK_COOLDOWN        { 42'000 };
    inline constexpr std::chrono::duration<double, std::milli> PLAYER_EJECT_COOLDOWN       { 7'000 };
    inline constexpr std::chrono::duration<double, std::milli> PLAYER_TAKE_COOLDOWN        { 7'000 };
    inline constexpr std::chrono::duration<double, std::milli> PLAYER_SET_COOLDOWN         { 7'000 };
    inline constexpr std::chrono::duration<double, std::milli> PLAYER_INCANTATION_COOLDOWN { 300'000 };
}
