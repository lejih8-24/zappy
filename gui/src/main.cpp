/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Program Entry
*/

#include "Window.hpp"
#include "Map.hpp"
#include "Camera.hpp"
#include "raylib.h"


int main()
{
    Window win(1280, 720, "Zappy GUI", 60);

    Map map(10, 10);

    GameCamera camera(
        { 0.0f, 20.0f, 20.0f },
        { 0.0f,  0.0f,  0.0f }
    );

    while (!win.shouldClose()) {
        camera.update();

        win.beginFrame();
            DrawFPS(10, 10);

            camera.begin3D();
                map.draw();
            camera.end3D();

            DrawText("Zappy GUI - 3D Map  |  [WASD] deplacer  [R] reset", 10, 34, 18, RAYWHITE);
        win.endFrame();
    }

    return 0;
}