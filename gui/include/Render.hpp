/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI render
*/

#pragma once

#include "Camera.hpp"
#include "Game/GameState.hpp"
#include "Map.hpp"
#include "Window.hpp"

#include <string_view>

namespace Zappy::Networking {
    class GraphicsClient;
}

namespace GUI {

class Render {
    public:
        Render(std::string_view host, int port, GameState &state);
        void renderLoop(Zappy::Networking::GraphicsClient &client);

    private:
        GameState &_state;
        Window _window;
        Map _map;
        GameCamera _camera;
};

}
