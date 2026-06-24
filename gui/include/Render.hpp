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
#include "Theme/ThemeManager.hpp"
#include "Window.hpp"

#include <string_view>

namespace Zappy::Networking {
    class GraphicsClient;
}

namespace GUI {

class Render {
    public:
        Render(std::string_view host, int port, std::string_view pack = "default");
        void renderLoop(Zappy::Networking::GraphicsClient &client);

    private:
        void drawHud() const;

        GameState _state;
        Window _window;
        ThemeManager _themeManager;
        Map _map;
        GameCamera _camera;
        bool _showHud = true;
};

}
