/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** ServerGameEnd event struct
*/

#pragma once

#include <string>


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "seg N\n"
     */
    struct ServerGameEnd {
        std::string winnerTeam;
    };
}
