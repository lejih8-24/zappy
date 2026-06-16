/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Server class
*/

#include "Server.hpp"
#include <iostream>


Zappy::Server::Server(std::string_view ip, std::uint16_t port)
   : Lattice::Server<Client>(ip, port)
{
   std::clog << "starting server on " << ip << ":" << port << std::endl;
}

Zappy::Server::~Server()
{
   std::clog << "shutting down server..." << std::endl;
}

void Zappy::Server::onClientAccepted(const Client& client)
{
   std::clog << std::string(client) << ": connected" << std::endl;
}

void Zappy::Server::onClientDisconnected(const Client& client)
{
   std::clog << "client disconnected" << std::endl;
}

void Zappy::Server::updateClient(Client& client)
{
   static std::string input;

   if (!client.readUntil(input, '\n'))
      return;

   std::clog << std::string(client) << ": " << input << std::endl;
}
