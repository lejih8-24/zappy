/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GameCamera wrapper around raylib Camera3D
*/

#include "Camera.hpp"
#include "raylib.h"

static constexpr float cameraMoveSpeed = 5.4F;
static constexpr float cameraPanSpeed = 2.0F;
static constexpr float cameraMouseSensitivity = 0.003F * RAD2DEG;

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
        updateFreeCamera();

    if (::IsKeyPressed(KEY_R))
        reset();
}

void GameCamera::updateFreeCamera()
{
    const float frameMoveSpeed = cameraMoveSpeed * ::GetFrameTime();
    const float framePanSpeed = cameraPanSpeed * ::GetFrameTime();
    const Vector2 mouseDelta = ::GetMouseDelta();

    // Inputs sent to raylib's custom camera update
    Vector3 movement = { 0.0F, 0.0F, 0.0F };
    Vector3 rotation = { 0.0F, 0.0F, 0.0F };
    float zoom = -::GetMouseWheelMove();

    // Movement handling
    if (::IsKeyDown(KEY_W))
        movement.x += frameMoveSpeed;
    if (::IsKeyDown(KEY_S))
        movement.x -= frameMoveSpeed;
    if (::IsKeyDown(KEY_D))
        movement.y += frameMoveSpeed;
    if (::IsKeyDown(KEY_A))
        movement.y -= frameMoveSpeed;
    if (::IsKeyDown(KEY_SPACE))
        movement.z += frameMoveSpeed;
    if (::IsKeyDown(KEY_LEFT_CONTROL))
        movement.z -= frameMoveSpeed;

    // Middle mouse pans the camera instead of rotating it
    if (::IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        if (mouseDelta.x > 0.0F)
            movement.y += framePanSpeed;
        if (mouseDelta.x < 0.0F)
            movement.y -= framePanSpeed;
        if (mouseDelta.y > 0.0F)
            movement.z -= framePanSpeed;
        if (mouseDelta.y < 0.0F)
            movement.z += framePanSpeed;
    } else {
        // Normal mouse movement rotates the camera
        rotation.x = mouseDelta.x * cameraMouseSensitivity;
        rotation.y = mouseDelta.y * cameraMouseSensitivity;
    }

    // Handle mouse wheel zoom
    if (::IsKeyPressed(KEY_KP_SUBTRACT))
        zoom += 2.0F;
    if (::IsKeyPressed(KEY_KP_ADD))
        zoom -= 2.0F;

    // Apply the custom controls without enabling raylib arrow-key rotation
    ::UpdateCameraPro(&_camera, movement, rotation, zoom);
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

bool GameCamera::isCursorLocked() const
{
    return ::IsCursorHidden();
}

}
