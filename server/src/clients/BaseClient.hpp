/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** BaseClient class
*/

#pragma once

#include "IClient.hpp"
#include <lattice.hpp>


namespace Zappy {
    class BaseClient : public IClient {
        protected:
            Lattice::CachingSocket<> m_Socket;

        public:
            BaseClient(Lattice::Socket&& socket);

            inline short requiredEvents() const override { return m_Socket.requiredEvents(); }
            inline void registerEvents(short revents) override { return m_Socket.registerEvents(revents); }
            inline int fileno() const override { return m_Socket.fileno(); }
            inline bool isOpen() const override { return m_Socket.isOpen(); }
    };
}
