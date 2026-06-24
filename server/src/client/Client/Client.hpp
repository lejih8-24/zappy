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


namespace Zappy::Client {
    class Client {
        Lattice::CachingSocket<> m_Socket;
        std::unique_ptr<IState> m_State;
        std::unique_ptr<IState> m_NextState;

        public:
            Client(Lattice::Socket&& socket);

            void update(Tick elapsedTicks);

            void setState(std::unique_ptr<IState>&& state) { m_NextState = std::move(state); }

            inline Lattice::CachingSocket<>& socket() { return m_Socket; }

            // Functions to satisfy ClientSocketType constraint
            inline short requiredEvents() const { return m_Socket.requiredEvents(); }
            inline void registerEvents(short revents) { return m_Socket.registerEvents(revents); }
            inline int fileno() const { return m_Socket.fileno(); }
            inline bool isOpen() const { return m_Socket.isOpen(); }
    };
}
