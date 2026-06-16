/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Server class
*/

#pragma once

#include <lattice/networking.hpp>


namespace Zappy {
    class Server : public Lattice::Server<Lattice::CachingSocket<>> {
        using Client = Lattice::CachingSocket<>;

        public:
            Server(std::string_view ip, std::uint16_t port);
            ~Server();

        private:
            void onClientAccepted(const Client& client) override;
            void onClientDisconnected(const Client& client) override;
            void updateClient(Client& client) override;
    };
}
