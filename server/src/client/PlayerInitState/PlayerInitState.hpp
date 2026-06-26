/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerInitState class
*/

#pragma once

#include "../QueueState.hpp"
#include <string>


namespace Zappy::Game {
    class Player;
}

namespace Zappy::Client {
    class PlayerInitState : public QueueState<128> {
        std::string m_TeamName;
        Game::Player* m_Player;

        public:
            PlayerInitState(const std::string& teamName);
            PlayerInitState(std::string&& teamName);

            void init() override;
            void update(Client& client, std::chrono::nanoseconds dt) override;
    };
}
