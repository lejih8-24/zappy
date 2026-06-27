/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GameCamera wrapper
*/

#include "Camera.hpp"

#include "raylib.h"
#include "raymath.h"

static constexpr float cameraMoveSpeed = 5.4F;
static constexpr float cameraPanSpeed = 2.0F;
static constexpr float cameraSprintMultiplier = 2.5F;
static constexpr float cameraMouseSensitivity = 0.003F * RAD2DEG;

struct GUI::GameCamera::CameraData {
    Camera3D camera = {};
    Vec3 initPosition = {};
    Vec3 initTarget = {};
};

static ::Vector3 toRaylibVec3(GUI::Vec3 value)
{
    return {value.x, value.y, value.z};
}

static GUI::Vec3 fromRaylibVec3(::Vector3 value)
{
    return {value.x, value.y, value.z};
}

namespace GUI {

GameCamera::GameCamera(Vec3 position, Vec3 target, float fovy)
    : _cameraData(std::make_unique<CameraData>())
{
    _cameraData->initPosition = position;
    _cameraData->initTarget = target;
    _cameraData->camera.position = toRaylibVec3(position);
    _cameraData->camera.target = toRaylibVec3(target);
    _cameraData->camera.up = {0.0f, 1.0f, 0.0f};
    _cameraData->camera.fovy = fovy;
    _cameraData->camera.projection = CAMERA_PERSPECTIVE;
    ::DisableCursor();
}

GameCamera::~GameCamera() = default;

void GameCamera::update(const IWindow &window)
{
    if (window.isKeyPressed(Key::Escape) && window.isCursorHidden())
        window.showCursor();
    else if (window.isMouseButtonPressed(MouseButton::Left) && !window.isCursorHidden())
        window.hideCursor();

    if (window.isCursorHidden())
        updateFreeCamera(window);

    if (window.isKeyPressed(Key::R))
        reset();
}

void GameCamera::updateFreeCamera(const IWindow &window)
{
    const bool isSprinting = window.isKeyDown(Key::LeftShift) || window.isKeyDown(Key::RightShift);
    const float speedMultiplier = isSprinting ? cameraSprintMultiplier : 1.0F;
    const float frameMoveSpeed = cameraMoveSpeed * speedMultiplier * window.frameTime();
    const float framePanSpeed = cameraPanSpeed * speedMultiplier * window.frameTime();
    const Vec2 mouseDelta = window.mouseDelta();

    // Rotation and zoom still use the backend's custom camera update.
    Vector3 rotation = {0.0F, 0.0F, 0.0F};
    float zoom = -window.mouseWheel();

    // Keyboard movement follows the camera direction instead of the world plane.
    if (window.isKeyDown(Key::W))
        moveCamera(getForwardVector(), frameMoveSpeed);
    if (window.isKeyDown(Key::S))
        moveCamera(getForwardVector(), -frameMoveSpeed);
    if (window.isKeyDown(Key::D))
        moveCamera(getRightVector(), frameMoveSpeed);
    if (window.isKeyDown(Key::A))
        moveCamera(getRightVector(), -frameMoveSpeed);
    if (window.isKeyDown(Key::Space))
        moveCamera(fromRaylibVec3(_cameraData->camera.up), frameMoveSpeed);
    if (window.isKeyDown(Key::LeftControl))
        moveCamera(fromRaylibVec3(_cameraData->camera.up), -frameMoveSpeed);

    // Middle mouse pans the camera instead of rotating it.
    if (window.isMouseButtonDown(MouseButton::Middle)) {
        if (mouseDelta.x > 0.0F)
            moveCamera(getRightVector(), framePanSpeed);
        if (mouseDelta.x < 0.0F)
            moveCamera(getRightVector(), -framePanSpeed);
        if (mouseDelta.y > 0.0F)
            moveCamera(fromRaylibVec3(_cameraData->camera.up), -framePanSpeed);
        if (mouseDelta.y < 0.0F)
            moveCamera(fromRaylibVec3(_cameraData->camera.up), framePanSpeed);
    } else {
        // Normal mouse movement rotates the camera.
        rotation.x = mouseDelta.x * cameraMouseSensitivity;
        rotation.y = mouseDelta.y * cameraMouseSensitivity;
    }

    // Handle keypad zoom.
    if (window.isKeyPressed(Key::KeypadSubtract))
        zoom += 2.0F;
    if (window.isKeyPressed(Key::KeypadAdd))
        zoom -= 2.0F;

    // Apply rotation and zoom without enabling arrow-key rotation.
    ::UpdateCameraPro(&_cameraData->camera, {0.0F, 0.0F, 0.0F}, rotation, zoom);
}

void GameCamera::moveCamera(Vec3 direction, float distance)
{
    const Vector3 move = ::Vector3Scale(toRaylibVec3(direction), distance);

    _cameraData->camera.position = ::Vector3Add(_cameraData->camera.position, move);
    _cameraData->camera.target = ::Vector3Add(_cameraData->camera.target, move);
}

Vec3 GameCamera::getForwardVector() const
{
    return fromRaylibVec3(::Vector3Normalize(::Vector3Subtract(_cameraData->camera.target, _cameraData->camera.position)));
}

Vec3 GameCamera::getRightVector() const
{
    return fromRaylibVec3(::Vector3Normalize(::Vector3CrossProduct(toRaylibVec3(getForwardVector()), _cameraData->camera.up)));
}

void GameCamera::begin3D() const
{
    ::BeginMode3D(_cameraData->camera);
}

void GameCamera::end3D() const
{
    ::EndMode3D();
}

void GameCamera::reset()
{
    _cameraData->camera.position = toRaylibVec3(_cameraData->initPosition);
    _cameraData->camera.target = toRaylibVec3(_cameraData->initTarget);
    _cameraData->camera.up = {0.0f, 1.0f, 0.0f};
}

bool GameCamera::isCursorLocked() const
{
    return ::IsCursorHidden();
}

std::optional<Vec2> GameCamera::projectToScreen(Vec3 worldPos) const
{
    Vector3 pos = toRaylibVec3(worldPos);
    Vector3 forward = Vector3Normalize(Vector3Subtract(_cameraData->camera.target, _cameraData->camera.position));

    // dot product <= 0 means the point is behind or perpendicular to the camera.
    if (Vector3DotProduct(forward, Vector3Subtract(pos, _cameraData->camera.position)) <= 0)
        return {};
    Vector2 screen = GetWorldToScreen(pos, _cameraData->camera);
    if (screen.x < 0 || screen.x > static_cast<float>(GetScreenWidth())
        || screen.y < 0 || screen.y > static_cast<float>(GetScreenHeight()))
        return {};
    return Vec2{screen.x, screen.y};
}

float GameCamera::distanceTo(Vec3 worldPos) const
{
    return Vector3Distance(_cameraData->camera.position, toRaylibVec3(worldPos));
}

std::optional<float> GameCamera::screenCenterHitDistance(Vec3 worldPos, float radius) const
{
    Vector2 center = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    Ray ray = GetMouseRay(center, _cameraData->camera);
    RayCollision hit = GetRayCollisionSphere(ray, toRaylibVec3(worldPos), radius);

    if (!hit.hit)
        return {};
    return hit.distance;
}

}
