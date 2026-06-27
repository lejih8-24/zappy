/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Client class
*/

#pragma once

#include "../IState.hpp"
#include <zappy/game.hpp>
#include <lattice.hpp>
#include <memory>
#include <chrono>
#include <string>
#include <deque>


namespace Zappy::Client {
    class Client {
        static constexpr std::size_t COMMAND_LIMIT = 10;

        Lattice::CachingSocket<> m_Socket;
        std::unique_ptr<IState> m_State;
        std::unique_ptr<IState> m_NextState;
        std::string m_ClientName;

        std::deque<std::string> m_QueuedCommands;

        public:
            Client(Lattice::Socket&& socket);
            Client(Client&& other);

            void update(Game::Game& game, std::chrono::nanoseconds dt);
            void updatePost();
            void handleDisconnect(Game::Game& game);

            void setState(std::unique_ptr<IState>&& state) { m_NextState = std::move(state); }
            bool readline(std::string& output);
            inline auto send(std::span<const char> data) { return m_Socket.write(data); }

            std::string_view name() const noexcept { return m_ClientName; }
            inline operator std::string() const { return m_ClientName; }

            inline void operator=(Client&& other) { swap(other); }
            void swap(Client& other);

            // Functions to satisfy ClientSocketType constraint
            inline short requiredEvents() const { return m_Socket.requiredEvents(); }
            inline void registerEvents(short revents) { return m_Socket.registerEvents(revents); }
            inline int fileno() const { return m_Socket.fileno(); }
            inline bool isOpen() const { return m_Socket.isOpen(); }
            inline void refresh() noexcept { m_Socket.refresh(); }

        private:
            void nextState(Game::Game& game);
            void pushCommand();
    };
}
