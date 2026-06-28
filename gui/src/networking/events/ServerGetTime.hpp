/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** ServerGetTime event struct
*/

#pragma once


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "sgt T\n"
     */
    struct ServerGetTime {
        unsigned int time;
    };
}
