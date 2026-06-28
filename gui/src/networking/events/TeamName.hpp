/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** TeamName event struct
*/

#pragma once

#include <string>


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "tna N\n"
     */
    struct TeamName {
        std::string name;
    };
}
