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

// May delete
// static GUI::GameState createDemoState()
// {
//     GUI::GameState state(10, 10, 100);
//     Zappy::Game::Resources resources = {};
//
//     resources[Zappy::Game::ResourceType::FOOD] = 3;
//     resources[Zappy::Game::ResourceType::LINEMATE] = 1;
//     state.tiles.emplace_back(4, 2, resources);
//     state.tiles.emplace_back(7, 7, resources);
//     state.players.emplace(1, GUI::Player(1, 4, 2, 1, "TeamA"));
//     state.eggs.emplace(1, GUI::Egg(1, 1, 6, 5));
//     state.teams.emplace_back("TeamA");
//     return state;
// }

// May delete
// class EventHandler : public Zappy::Networking::BaseEventHandler {
//     public:
//         using Zappy::Networking::BaseEventHandler::BaseEventHandler;
//         using Zappy::Networking::BaseEventHandler::operator();
//
//         void operator()(const Zappy::Networking::TileContents &tile)
//         {
//             std::cout << "tile[" << tile.x << ", " << tile.y << "]: " << tile.resources
//                       << std::endl;
//         }
// };

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

        // May delete
        // auto event = client.pollEvent();
        // while (event) {
        //     std::visit([](auto &value) {
        //         std::cout << typeid(value).name() << std::endl;
        //     }, *event);
        //     event = client.pollEvent();
        // }

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
