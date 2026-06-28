/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerExpulsion event struct
*/

#pragma once


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "pex #n\n"
     */
    struct PlayerExpulsion {
        int id;
    };
}
