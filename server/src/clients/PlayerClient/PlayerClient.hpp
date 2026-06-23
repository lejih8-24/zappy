/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerClient class
*/

#pragma once

#include "../BaseClient.hpp"
#include <zappy/game.hpp>


namespace Zappy {
    class PlayerClient : public BaseClient {
        std::optional<Game::Player> m_PlayerData;

        public:
            PlayerClient(Lattice::Socket&& socket);

            const Game::Player& data() const { return *m_PlayerData; }

            void update(Tick elapsedTicks) override;

        private:
            void completeHandshake();
    };
}
