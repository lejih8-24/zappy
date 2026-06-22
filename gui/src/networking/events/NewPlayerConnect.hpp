/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** NewPlayerConnect event struct
*/

#pragma once

#include <string>


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "pnw #n X Y O L N\n"
     */
    struct NewPlayerConnect {
        int id;
        unsigned int x;
        unsigned int y;
        float rotationDeg;
        unsigned int level;
        std::string teamName;
    };
}
