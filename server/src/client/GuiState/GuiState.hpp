/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** GuiState class
*/

#pragma once

#include "../QueueState.hpp"


namespace Zappy::Client {
    class GuiState : public QueueState<128> {
        public:
            GuiState();

            void init() override;
            void update(Client& client, std::chrono::nanoseconds dt) override;

        private:
            void handleCommand(std::string_view command);
    };
}
