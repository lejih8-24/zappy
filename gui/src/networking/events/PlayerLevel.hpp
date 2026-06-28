/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerLevel event struct
*/

#pragma once


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "plv #n N\n"
     */
    struct PlayerLevel {
        int id;
        unsigned int level;
    };
}
