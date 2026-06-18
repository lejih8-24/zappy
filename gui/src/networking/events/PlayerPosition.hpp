/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerPosition event struct
*/

#pragma once


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "ppo #n X Y O\n"
     */
    struct PlayerPosition {
        int id;
        unsigned int x;
        unsigned int y;
        float rotationDeg;
    };
}
