/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerIncantationStart event struct
*/

#pragma once

#include <vector>


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "pic X Y L #n #n ...\n"
     */
    struct PlayerIncantationStart {
        unsigned int x;
        unsigned int y;
        unsigned int level;
        std::vector<int> playerIds;
    };
}
