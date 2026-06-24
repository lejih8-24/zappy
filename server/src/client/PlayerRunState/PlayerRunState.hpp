/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerRunState class
*/

#pragma once

#include "../BaseState.hpp"


namespace Zappy::Client {
    class PlayerRunState : public BaseState {
        public:
            PlayerRunState();

            void update(Client& client, std::chrono::nanoseconds dt) override;
    };
}
