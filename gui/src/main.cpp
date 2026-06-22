/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Program Entry
*/

#include "GuiArgs.hpp"
#include "Game/GameState.hpp"
#include "Render.hpp"
#include "exceptions/ArgsException.hpp"
#include "game/Resources/Resources.hpp"
#include <networking.hpp>
#include "raylib.h"
#include <iostream>

int main(int argc, char *argv[])
{
    #ifdef NDEBUG
    ::SetTraceLogLevel(LOG_NONE);
    #endif

    try {
        const GUI::GuiArgs args = GUI::GuiArgs::parseArgs(argc, argv);
        GUI::GameState state;

        Zappy::Networking::GraphicsClient client(args.getHost(), args.getPort());

        if (!client.isConnected()) {
            std::cerr << argv[0] << ": failed to connect to server\n";
            return 84;
        }

        GUI::Render render(args.getHost(), args.getPort(), state);

        render.renderLoop(client);
    } catch (const Zappy::Exceptions::ArgsException &error) {
        std::cerr << argv[0] << ": " << error.what() << '\n';
        GUI::GuiArgs::printUsage(std::cerr);
        return 84;
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return 84;
    } catch (...) {
        return 84;
    }
}
