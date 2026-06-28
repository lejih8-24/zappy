/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** EggCreate event struct
*/

#pragma once


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "enw #e #n X Y\n"
     */
    struct EggCreate {
        int eggId;
        int playerId;
        unsigned int x;
        unsigned int y;
    };
}
