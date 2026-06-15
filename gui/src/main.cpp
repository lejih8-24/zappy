/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Program Entry
*/

#include "GuiArgs.hpp"
#include "Render.hpp"
#include "exceptions/ArgsException.hpp"

#include <iostream>

int main(int argc, char **argv)
{
    try {
        const GUI::GuiArgs args = GUI::GuiArgs::parseArgs(argc, argv);
        GUI::Render render(args.getHost(), args.getPort());

        args.connect();
        render.renderLoop();
        return 0;
    } catch (const Zappy::Exceptions::ArgsException &error) {
        std::cerr << "zappy_gui: " << error.what() << '\n';
        GUI::GuiArgs::printUsage(std::cerr);
        return 84;
    }
}
