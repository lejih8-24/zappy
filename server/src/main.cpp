/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Program Entry
*/

#include <zappy.hpp>
#include <iostream>


void run(int argc, char *argv[])
{
    #ifdef NDEBUG
    Zappy::logger.setLevel(Zappy::Utils::Logger::WARNING);
    #endif

    auto server = Zappy::Server::Builder()
        .setHostname("localhost")
        .fromArguments(argc, argv)
        .build();

    server.run();
}

int main(int argc, char *argv[])
{
    try {
        run(argc, argv);
    } catch (const Zappy::Exceptions::BaseException& err) {
        std::cerr << err.what() << std::endl;
        return err.status();
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 84;
    } catch (...) {
        return 84;
    }
}
