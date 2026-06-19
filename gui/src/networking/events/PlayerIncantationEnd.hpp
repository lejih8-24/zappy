/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerIncantationEnd event struct
*/

#pragma once

#include <string>


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "pie X Y R\n"
     */
    struct PlayerIncantationEnd {
        unsigned int x;
        unsigned int y;
        std::string result;  // TODO: ensure result is a string
    };
}
