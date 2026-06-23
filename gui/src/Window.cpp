/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Window wrapper around raylib
*/

#include "Window.hpp"

namespace GUI {

Window::Window(int width, int height, const std::string &title, int fps)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, title.c_str());
    SetExitKey(KEY_X);
    SetTargetFPS(fps);
}

Window::~Window()
{
    CloseWindow();
}

bool Window::shouldClose() const
{
    return WindowShouldClose();
}

void Window::beginFrame()
{
    BeginDrawing();
    ClearBackground(DARKBLUE);
}

void Window::endFrame()
{
    EndDrawing();
}

void Window::toggleFullscreen()
{
    ToggleFullscreen();
}

}
