/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** GuiState class
*/

#pragma once

#include "../IState.hpp"


namespace Zappy::Client {
    class GuiState : public IState {
        Client& m_Client;

        public:
            GuiState();

            void update(Client& client, std::chrono::nanoseconds dt) override;
    };
}
