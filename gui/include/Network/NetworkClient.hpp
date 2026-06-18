/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** NetworkClient.hpp
*/

#pragma once

#include <string>

namespace GUI {
    class NetworkClient {
        public:
            NetworkClient(const std::string &host, int port);
            ~NetworkClient();

            bool connect();
            std::string readLine();
            void sendMessage(const std::string &msg);

        private:
            std::string _host;
            int _port;
            int _fd = -1;
    };
}