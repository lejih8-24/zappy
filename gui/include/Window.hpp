/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Window wrapper
*/

#pragma once

#include "Graphics/IWindow.hpp"
#include <string>

namespace GUI {

class Window : public IWindow {
    public:
        Window(int width, int height, const std::string &title, int fps = 60);
        ~Window() override;

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        bool shouldClose() const override;

        static void disableLogs();

        void beginFrame(Color background = Colors::DarkBlue) override;
        void endFrame() override;
        void toggleFullscreen() override;

        float now() const override;
        float frameTime() const override;
        bool isKeyPressed(Key key) const override;
        bool isKeyDown(Key key) const override;
        bool isMouseButtonPressed(MouseButton button) const override;
        bool isMouseButtonDown(MouseButton button) const override;
        Vec2 mouseDelta() const override;
        float mouseWheel() const override;
        void hideCursor() const override;
        void showCursor() const override;
        bool isCursorHidden() const override;
};

}
