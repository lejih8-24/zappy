/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** TileContents event struct
*/

#pragma once

#include <game.hpp>


namespace Zappy::Networking {
    /**
     * Related GUI protocol command:
     * "bct X Y q0 q1 q2 q3 q4 q5 q6\n"
     */
    struct TileContents {
        unsigned int x;
        unsigned int y;
        Game::Resources resources;
    };
}
