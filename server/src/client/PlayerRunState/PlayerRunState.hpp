/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerRunState class
*/

#pragma once

#include "../QueueState.hpp"
#include <unordered_map>
#include <string>


namespace Zappy::Game {
    class Player;
}

namespace Zappy::Client {
    class PlayerRunState : public QueueState<128> {
        using CommandHandler = void(*)(PlayerRunState&, Client&, Game::Game&);
        static const std::unordered_map<std::string_view, CommandHandler> COMMAND_HANDLERS;

        Game::Player& m_Player;
        std::chrono::duration<double, std::milli> m_Cooldown;

        public:
            PlayerRunState(Game::Player& player);

            void update(Client& client, std::chrono::nanoseconds dt) override;
            void disconnect(Game::Game& game) override;

        private:
            static void toLowercase(std::string& repr);
            void runCommand(Client& client, std::string& command);

            static void forwardCommand(PlayerRunState& state, Client& client, Game::Game& game);
            static void rightCommand(PlayerRunState& state, Client& client, Game::Game& game);
            static void leftCommand(PlayerRunState& state, Client& client, Game::Game& game);

            static void lookCommand(PlayerRunState& state, Client& client, Game::Game& game);
            static void inventoryCommand(PlayerRunState& state, Client& client, Game::Game& game);
            static void broadcastCommand(PlayerRunState& state, Client& client, Game::Game& game);

            static void connectNbrCommand(PlayerRunState& state, Client& client, Game::Game& game);

            static void forkCommand(PlayerRunState& state, Client& client, Game::Game& game);
            static void ejectCommand(PlayerRunState& state, Client& client, Game::Game& game);

            static void takeCommand(PlayerRunState& state, Client& client, Game::Game& game);
            static void incantationCommand(PlayerRunState& state, Client& client, Game::Game& game);
    };
}
