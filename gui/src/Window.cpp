/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Window wrapper around raylib
*/

#include "Window.hpp"

#include "raylib.h"

static ::Color toRaylibColor(GUI::Color color)
{
    return {color.r, color.g, color.b, color.a};
}

static int toRaylibKey(GUI::Key key)
{
    switch (key) {
        case GUI::Key::Escape:        return KEY_ESCAPE;
        case GUI::Key::F11:           return KEY_F11;
        case GUI::Key::H:             return KEY_H;
        case GUI::Key::Right:         return KEY_RIGHT;
        case GUI::Key::Left:          return KEY_LEFT;
        case GUI::Key::Down:          return KEY_DOWN;
        case GUI::Key::Up:            return KEY_UP;
        case GUI::Key::R:             return KEY_R;
        case GUI::Key::W:             return KEY_W;
        case GUI::Key::A:             return KEY_A;
        case GUI::Key::S:             return KEY_S;
        case GUI::Key::D:             return KEY_D;
        case GUI::Key::Space:         return KEY_SPACE;
        case GUI::Key::LeftControl:   return KEY_LEFT_CONTROL;
        case GUI::Key::LeftShift:     return KEY_LEFT_SHIFT;
        case GUI::Key::RightShift:    return KEY_RIGHT_SHIFT;
        case GUI::Key::KeypadAdd:     return KEY_KP_ADD;
        case GUI::Key::KeypadSubtract:return KEY_KP_SUBTRACT;
    }
    return KEY_NULL;
}

static int toRaylibMouseButton(GUI::MouseButton button)
{
    switch (button) {
        case GUI::MouseButton::Left:   return MOUSE_BUTTON_LEFT;
        case GUI::MouseButton::Middle: return MOUSE_BUTTON_MIDDLE;
    }
    return MOUSE_BUTTON_LEFT;
}

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

void Window::disableLogs()
{
    SetTraceLogLevel(LOG_NONE);
}

void Window::beginFrame(Color background)
{
    BeginDrawing();
    ClearBackground(toRaylibColor(background));
}

void Window::endFrame()
{
    EndDrawing();
}

void Window::toggleFullscreen()
{
    ToggleFullscreen();
}

float Window::now() const
{
    return static_cast<float>(GetTime());
}

float Window::frameTime() const
{
    return GetFrameTime();
}

bool Window::isKeyPressed(Key key) const
{
    return IsKeyPressed(toRaylibKey(key));
}

bool Window::isKeyDown(Key key) const
{
    return IsKeyDown(toRaylibKey(key));
}

bool Window::isMouseButtonPressed(MouseButton button) const
{
    return IsMouseButtonPressed(toRaylibMouseButton(button));
}

bool Window::isMouseButtonDown(MouseButton button) const
{
    return IsMouseButtonDown(toRaylibMouseButton(button));
}

Vec2 Window::mouseDelta() const
{
    ::Vector2 delta = GetMouseDelta();
    return {delta.x, delta.y};
}

float Window::mouseWheel() const
{
    return GetMouseWheelMove();
}

void Window::hideCursor() const
{
    DisableCursor();
}

void Window::showCursor() const
{
    EnableCursor();
}

bool Window::isCursorHidden() const
{
    return IsCursorHidden();
}

}
