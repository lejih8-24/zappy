/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** GraphicsClient class
*/

#pragma once

#include <lattice.hpp>
#include <string_view>
#include <optional>
#include <cstdint>
#include <vector>


namespace Zappy::Networking {
    struct Position {
        unsigned int x;
        unsigned int y;
    };

    class GraphicsClient {
        using coordinate = unsigned int;

        Lattice::Socket m_Server;
        // std::deque<> m_EventQueue;
        std::vector<std::string> m_TeamNames;

        public:
            GraphicsClient();
            GraphicsClient(std::string_view ip, std::uint16_t port);
            GraphicsClient(GraphicsClient&& other);

            bool isConnected() const { return m_Server.isOpen(); }

            Position mapSize();
            void tileContents(coordinate x, coordinate y);
            void mapContents();

            const std::vector<std::string>& teamNames();

            Position playerPosition(unsigned int playerId);
            unsigned int playerLevel(unsigned int playerId);
            void playerInventory(unsigned int playerId);

            unsigned int getTime();

            std::optional<int> pollEvent();

            inline void operator=(GraphicsClient&& other) { swap(other); }
            void swap(GraphicsClient& other);

        private:
            void doHandshake();

            std::string_view getline(bool wait = true);
            void send(std::string msg);
    };
}
