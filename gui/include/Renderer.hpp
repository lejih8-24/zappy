/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI renderer
*/

#pragma once

#include "Camera.hpp"
#include "Map.hpp"
#include "Window.hpp"

namespace GUI {

class Renderer {
    public:
        Renderer();
        void renderLoop();

    private:
        Window _window;
        Map _map;
        GameCamera _camera;
};

}
