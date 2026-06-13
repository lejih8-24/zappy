/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Program Entry
*/

#include "Window.hpp"
#include "Map.hpp"
#include "raylib.h"


int main()
{
    Window win(1280, 720, "Zappy GUI", 60);

    Map map(10, 10);

    Camera3D camera = {};
    camera.position   = { 0.0f, 20.0f, 20.0f };
    camera.target     = { 0.0f,  0.0f,  0.0f };
    camera.up         = { 0.0f,  1.0f,  0.0f };
    camera.fovy       = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!win.shouldClose()) {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        win.beginFrame();
            DrawFPS(10, 10);

            BeginMode3D(camera);
                map.draw();
            EndMode3D();

            DrawText("Zappy GUI - 3D Map", 10, 34, 20, RAYWHITE);
        win.endFrame();
    }

    return 0;
}