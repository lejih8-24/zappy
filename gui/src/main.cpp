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
#include "networking/GraphicsClient/GraphicsClient.hpp"
#include <game.hpp>

#include <iostream>

namespace {

//? Fonction temporaire où on peut hardcoder le GameState pour pouvoir tester et développer le render
//? Plus tard, on pourra créer directement le GameState par rapport à ce que nous enverra le serv
GUI::GameState createDemoState()
{
    GUI::GameState state(10, 10, 100);
    GUI::ResourceStock resources = {};

    resources[static_cast<std::size_t>(GUI::Resource::Food)] = 3;
    resources[static_cast<std::size_t>(GUI::Resource::Linemate)] = 1;
    state.tiles.emplace_back(4, 2, resources);
    state.tiles.emplace_back(7, 7, resources);
    state.players.emplace(1, GUI::Player(1, 4, 2, 1, "TeamA"));
    state.eggs.emplace(1, GUI::Egg(1, 1, 6, 5));
    state.teams.emplace_back("TeamA");
    return state;
}

}

int main(int argc, char **argv)
{
    try {
        const GUI::GuiArgs args = GUI::GuiArgs::parseArgs(argc, argv);
        GUI::GameState state = createDemoState();
        GUI::Render render(args.getHost(), args.getPort(), state);

        Zappy::Networking::GraphicsClient client("127.0.0.1", 5000);
        auto [x, y] = client.mapSize();
        std::cout << "map size: { " << x << "; " << y << " }" << std::endl;

        args.connect();
        render.renderLoop();
        return 0;
    } catch (const Zappy::Exceptions::ArgsException &error) {
        std::cerr << "zappy_gui: " << error.what() << '\n';
        GUI::GuiArgs::printUsage(std::cerr);
        return 84;
    }
}
