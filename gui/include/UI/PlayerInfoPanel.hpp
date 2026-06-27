/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Overlay panel displaying selected player info
*/

#pragma once

#include "Game/Player.hpp"

namespace GUI {

class PlayerInfoPanel {
    public:
        void draw(const Player &player) const;
};

}
