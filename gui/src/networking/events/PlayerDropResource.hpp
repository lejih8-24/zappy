/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerDropResource event struct
*/

#pragma once

#include <game.hpp>


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "pdr #n i\n"
     */
    struct PlayerDropResource {
        int playerId;
        Game::ResourceType resource;
    };
}
