/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** ServerMessage event struct
*/

#pragma once

#include <string>


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "smg N\n"
     */
    struct ServerMessage {
        std::string winnerTeam;
    };
}
