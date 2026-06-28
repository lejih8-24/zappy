/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** MapSize event struct
*/

#pragma once


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "msz X Y\n"
     */
    struct MapSize {
        unsigned int x;
        unsigned int y;
    };
}
