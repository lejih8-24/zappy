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


namespace Zappy::Client {
    class Client {
        Lattice::CachingSocket<> m_Socket;
        std::unique_ptr<IState> m_State;
        std::unique_ptr<IState> m_NextState;
        std::string m_ClientName;

        public:
            Client(Lattice::Socket&& socket);
            Client(Client&& other);

            void update(std::chrono::nanoseconds dt);

            void setState(std::unique_ptr<IState>&& state) { m_NextState = std::move(state); }

            inline Lattice::CachingSocket<>& socket() { return m_Socket; }

            std::string_view name() const noexcept { return m_ClientName; }
            inline operator std::string() const { return m_ClientName; }

            inline void operator=(Client&& other) { swap(other); }
            void swap(Client& other);

            // Functions to satisfy ClientSocketType constraint
            inline short requiredEvents() const { return m_Socket.requiredEvents(); }
            inline void registerEvents(short revents) { return m_Socket.registerEvents(revents); }
            inline int fileno() const { return m_Socket.fileno(); }
            inline bool isOpen() const { return m_Socket.isOpen(); }
    };
}
