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
    #ifdef NDEBUG
    Zappy::logger.setLevel(Zappy::Utils::Logger::WARNING);
    #endif

    try {
        auto server = Zappy::Server::Builder()
            .fromArguments(argc, argv)
            .build();

        server.run();
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 84;
    } catch (...) {
        return 84;
    }
}
