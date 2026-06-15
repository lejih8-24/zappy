/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI render
*/

#pragma once

#include "Camera.hpp"
#include "Map.hpp"
#include "Window.hpp"

#include <string>

namespace GUI {

class Render {
    public:
        Render(const std::string &host, int port);
        void renderLoop();

    private:
        Window _window;
        Map _map;
        GameCamera _camera;
};

}
