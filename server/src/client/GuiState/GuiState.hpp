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
        using CommandHandler = void(*)(std::string_view, GuiState&, Client&);
        static const std::unordered_map<std::string_view, CommandHandler> COMMAND_HANDLERS;

        public:
            GuiState();

            void init() override;
            inline void update(Client&, std::chrono::nanoseconds) override {}
            void postUpdate(Client& client) override;

        private:
            void handleCommand(std::string_view command, Client& client);

            static void mapSizeCommand(std::string_view args, GuiState& state, Client& client);
            static void tileContentsCommand(std::string_view args, GuiState& state, Client& client);
            static void mapContentsCommand(std::string_view args, GuiState& state, Client& client);
            static void teamNamesCommand(std::string_view args, GuiState& state, Client& client);
            static void playerPositionCommand(std::string_view args, GuiState& state, Client& client);
            static void playerLevelCommand(std::string_view args, GuiState& state, Client& client);
            static void playerInventoryCommand(std::string_view args, GuiState& state, Client& client);
            static void serverGetTimeCommand(std::string_view args, GuiState& state, Client& client);
            static void serverSetTimeCommand(std::string_view args, GuiState& state, Client& client);

            static std::optional<std::pair<unsigned int, unsigned int>> parsePosition(std::string_view& str);
    };
}
