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
#include <memory>


namespace Zappy {
    class PlayerClient : public BaseClient {
        private:
            struct State {
                virtual void update(Tick elapsedTicks) = 0;
            };

            static inline constexpr std::size_t MAX_BUFFERED_COMMANDS = 10;

        private:
            std::optional<Game::Player> m_PlayerData;
            std::unique_ptr<State> m_ClientState;

        public:
            PlayerClient(Lattice::Socket&& socket);

            const Game::Player& data() const { return *m_PlayerData; }

            void update(Tick elapsedTicks) override;

        private:
            struct HandshakeState : State {
                PlayerClient& client;
                std::string teamName;

                inline HandshakeState(PlayerClient& client) : client(client) {}
                void update(Tick elapsedTime) override;
            };

            struct DefaultState : State {
                PlayerClient& client;
                inline DefaultState(PlayerClient& client) : client(client) {}
                void update(Tick elapsedTime) override;
            };

            struct WaitingState : State {
                PlayerClient& client;
                inline WaitingState(PlayerClient& client) : client(client) {}
                void update(Tick) override;
            };
    };
}
