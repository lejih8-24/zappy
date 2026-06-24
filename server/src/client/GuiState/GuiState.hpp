/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** GuiState class
*/

#pragma once

#include "../BaseState.hpp"


namespace Zappy::Client {
    class GuiState : public BaseState {
        public:
            GuiState();

            void update(Client& client, std::chrono::nanoseconds dt) override;
    };
}
