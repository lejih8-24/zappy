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

namespace GUI {

class Render {
    public:
        Render();
        void renderLoop();

    private:
        Window _window;
        Map _map;
        GameCamera _camera;
};

}
