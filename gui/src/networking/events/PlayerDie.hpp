/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerDie event struct
*/

#pragma once


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "pdi #n\n"
     */
    struct PlayerDie {
        int playerId;
    };
}
