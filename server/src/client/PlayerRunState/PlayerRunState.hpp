/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerRunState class
*/

#pragma once

#include "../IState.hpp"


namespace Zappy::Client {
    class PlayerRunState : public IState {
        public:
            PlayerRunState();

            void update(Client& client, std::chrono::nanoseconds dt) override;
    };
}
