/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Program Entry
*/

#include <zappy.hpp>
#include <iostream>


void runServer()
{

}

int main(int argc, char *argv[])
{
    try {
        runServer();
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 84;
    } catch (...) {
        return 84;
    }
}
