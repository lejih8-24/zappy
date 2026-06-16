/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Program Entry
*/

#include <zappy.hpp>
#include <iostream>


int main(int argc, char *argv[])
{
    try {
        Zappy::Server server("127.0.0.1", 5001);
        server.run();
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 84;
    } catch (...) {
        return 84;
    }
}
