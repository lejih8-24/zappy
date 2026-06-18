/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** NetworkClient.cpp
*/

#include "Network/NetworkClient.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>

namespace GUI {

    NetworkClient::NetworkClient(const std::string &host, int port) : _host(host), _port(port)
    {

    }

    NetworkClient::~NetworkClient()
    {
        if (_fd != -1) {
            close(_fd);
        }
    }

    bool NetworkClient::connect()
    {
        struct hostent *server = gethostbyname(_host.c_str());

        if (!server) {
            return false;
        }
        _fd = socket(AF_INET, SOCK_STREAM, 0);
        if (_fd < 0) {
            return false;
        }

        struct sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(_port);
        addr.sin_addr = *reinterpret_cast<struct in_addr *>(server->h_addr);

        if (::connect(_fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
            return false;
        }
        return true;
    }

    std::string NetworkClient::readLine()
    {
        std::string line;
        char c = 0;

        while (read(_fd, &c, 1) > 0 && c != '\n') {
            line += c;
        }
        return line;
    }

    void NetworkClient::sendMessage(const std::string &msg)
    {
        write(_fd, msg.c_str(), msg.size());
    }
}