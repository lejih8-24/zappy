/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerBroadcast event struct
*/

#pragma once

#include <string>


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "pbc #n M\n"
     */
    struct PlayerBroadcast {
        int id;
        std::string message;
    };
}
