/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GameCamera wrapper around raylib Camera3D
*/

#include "Camera.hpp"
#include "raylib.h"

namespace GUI {

GameCamera::GameCamera(Vector3 position, Vector3 target, float fovy)
    : _initPosition(position)
    , _initTarget(target)
{
    _camera.position   = position;
    _camera.target     = target;
    _camera.up         = { 0.0f, 1.0f, 0.0f };
    _camera.fovy       = fovy;
    _camera.projection = CAMERA_PERSPECTIVE;
    ::DisableCursor();
}

void GameCamera::update()
{
    if (::IsKeyPressed(KEY_ESCAPE) && ::IsCursorHidden())
        ::EnableCursor();
    else if (::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !::IsCursorHidden())
        ::DisableCursor();

    if (::IsCursorHidden())
        ::UpdateCamera(&_camera, CAMERA_FREE);

    if (::IsKeyPressed(KEY_R))
        reset();
}

void GameCamera::begin3D() const
{
    ::BeginMode3D(_camera);
}

void GameCamera::end3D() const
{
    ::EndMode3D();
}

void GameCamera::reset()
{
    _camera.position = _initPosition;
    _camera.target   = _initTarget;
    _camera.up       = { 0.0f, 1.0f, 0.0f };
}

}
