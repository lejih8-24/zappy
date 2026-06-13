/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Window wrapper around raylib
*/

#include "Window.hpp"

Window::Window(int width, int height, const std::string &title, int fps)
{
    InitWindow(width, height, title.c_str());
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
