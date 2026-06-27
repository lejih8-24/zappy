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
            inline void update(Client&, std::chrono::nanoseconds) override {}
            void postUpdate(Client& client) override;

        private:
            void handleCommand(std::string_view command);
    };
}
