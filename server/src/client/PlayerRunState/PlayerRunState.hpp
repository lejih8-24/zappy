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
        using CommandHandler = void(*)(std::string_view, PlayerRunState&, Client&, Game::Game&);
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

            static void forwardCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game);
            static void rightCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game);
            static void leftCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game);

            static void lookCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game);
            static void inventoryCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game);
            static void broadcastCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game);

            static void connectNbrCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game);

            static void forkCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game);
            static void ejectCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game);

            static void takeCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game);
            static void setCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game);
            static void incantationCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game);
    };
}
