/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** GuiState class
*/

#pragma once

#include "../BaseState.hpp"
#include <deque>


namespace Zappy::Client {
    class GuiState : public BaseState {
        std::deque<std::string> m_QueuedEvents;

        public:
            GuiState();

            void init() override;
            void update(Client& client, std::chrono::nanoseconds dt) override;

        private:
            void handleCommand(std::string_view command);
    };
}
