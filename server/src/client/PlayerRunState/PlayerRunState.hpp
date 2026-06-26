/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerRunState class
*/

#pragma once

#include "../QueueState.hpp"
#include <string>


namespace Zappy::Game {
    class Player;
}

namespace Zappy::Client {
    class PlayerRunState : public QueueState<128> {
        std::string m_TeamName;
        Game::Player* m_Player;

        public:
            PlayerRunState(const std::string& teamName);
            PlayerRunState(std::string&& teamName);

            void init() override;
            void update(Client& client, std::chrono::nanoseconds dt) override;
    };
}
