/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerWaitState class
*/

#pragma once

#include "../QueueState.hpp"


namespace Zappy::Game {
    class Player;
}

namespace Zappy::Client {
    class PlayerWaitState : public QueueState<128> {
            Game::Player& m_Player;
            std::chrono::duration<double, std::milli> m_WaitDuration;
            std::string m_EndMessage;

        public:
            PlayerWaitState(Game::Player& player, std::chrono::duration<double, std::milli> period, std::string_view endMessage = std::string_view());

            void update(Client& client, std::chrono::nanoseconds dt) override;
    };
}
