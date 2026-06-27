/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Window wrapper
*/

#pragma once

#include "Graphics/Types.hpp"
#include <string>

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

class Window {
    public:
        Window(int width, int height, const std::string &title, int fps = 60);
        ~Window();
    
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
    
        bool shouldClose() const;
    
        static void disableLogs();

        void beginFrame(Color background = Colors::DarkBlue);
        void endFrame();
        void toggleFullscreen();

        float now() const;
        float frameTime() const;
        bool isKeyPressed(Key key) const;
        bool isKeyDown(Key key) const;
        bool isMouseButtonPressed(MouseButton button) const;
        bool isMouseButtonDown(MouseButton button) const;
        Vec2 mouseDelta() const;
        float mouseWheel() const;
        void hideCursor() const;
        void showCursor() const;
        bool isCursorHidden() const;
};

}
