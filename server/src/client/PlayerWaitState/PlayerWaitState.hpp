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

        public:
            PlayerWaitState(Game::Player& player);
            PlayerWaitState(Game::Player& player, QueueState& queue);

            void update(Client& client, std::chrono::nanoseconds dt) override;
            void disconnect(Game::Game& game) override;
    };
}
