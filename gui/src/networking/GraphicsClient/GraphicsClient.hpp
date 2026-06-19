/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** GraphicsClient class
*/

#pragma once

#include "../Event.hpp"
#include <utils.hpp>
#include <lattice.hpp>
#include <string_view>
#include <optional>
#include <cstdint>
#include <vector>
#include <deque>


namespace Zappy::Networking {
    class GraphicsClient {
        using coordinate = unsigned int;

        Lattice::Socket m_Server;
        Utils::LineBuffer<4096> m_ResponseBuffer;
        std::deque<Event> m_EventQueue;

        public:
            GraphicsClient();
            GraphicsClient(std::string_view ip, std::uint16_t port);
            GraphicsClient(GraphicsClient&& other);

            bool isConnected() const { return m_Server.isOpen(); }

            MapSize mapSize();
            TileContents tileContents(coordinate x, coordinate y);
            void mapContents();
            void teamNames(std::vector<std::string>& names);

            PlayerPosition playerPosition(unsigned int playerId);
            unsigned int playerLevel(unsigned int playerId);
            void playerInventory(unsigned int playerId);

            unsigned int getTime();
            void setTime(unsigned int units);

            std::optional<Event> pollEvent();

            inline void operator=(GraphicsClient&& other) { swap(other); }
            void swap(GraphicsClient& other);

        private:
            void refreshEvents();
            void doHandshake();

            static bool isValidResponse(std::string_view response);

            std::string_view getline(bool wait = true);
            std::string_view getResponse(std::string_view cmd);
            void send(std::string_view msg);
    };
}
