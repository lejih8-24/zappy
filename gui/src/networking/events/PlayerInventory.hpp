/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerInventory event struct
*/

#pragma once

#include <game.hpp>


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "pin #n X Y q0 q1 q2 q3 q4 q5 q6\n"
     */
    struct PlayerInventory {
        int id;
        unsigned int x;
        unsigned int y;
        Game::Resources inventory;
    };
}
