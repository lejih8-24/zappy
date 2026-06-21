/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** ServerSetTime event struct
*/

#pragma once


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "sst T\n"
     */
    struct ServerSetTime {
        unsigned int time;
    };
}
