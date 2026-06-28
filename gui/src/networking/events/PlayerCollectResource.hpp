/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerCollectResource event struct
*/

#pragma once

#include <game.hpp>


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "pgt #n i\n"
     */
    struct PlayerCollectResource {
        int playerId;
        Game::ResourceType resource;
    };
}
