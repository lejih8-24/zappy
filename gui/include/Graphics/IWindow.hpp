/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Window/input abstraction (backend-agnostic)
*/

#pragma once

#include "Graphics/Types.hpp"

namespace GUI {

enum class Key {
    Escape,
    F11,
    H,
    Right,
    Left,
    Down,
    Up,
    R,
    W,
    A,
    S,
    D,
    Space,
    LeftControl,
    LeftShift,
    RightShift,
    KeypadAdd,
    KeypadSubtract,
};

enum class MouseButton {
    Left,
    Middle,
};

class IWindow {
    public:
        virtual ~IWindow() = default;

        virtual bool shouldClose() const = 0;

        virtual void beginFrame(Color background = Colors::DarkBlue) = 0;
        virtual void endFrame() = 0;
        virtual void toggleFullscreen() = 0;

        virtual float now() const = 0;
        virtual float frameTime() const = 0;
        virtual bool isKeyPressed(Key key) const = 0;
        virtual bool isKeyDown(Key key) const = 0;
        virtual bool isMouseButtonPressed(MouseButton button) const = 0;
        virtual bool isMouseButtonDown(MouseButton button) const = 0;
        virtual Vec2 mouseDelta() const = 0;
        virtual float mouseWheel() const = 0;
        virtual void hideCursor() const = 0;
        virtual void showCursor() const = 0;
        virtual bool isCursorHidden() const = 0;
};

}
