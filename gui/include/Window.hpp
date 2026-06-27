/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Window wrapper around raylib
*/

#pragma once

#include "raylib.h"
#include <string>

namespace GUI {

class Window {
    public:
        Window(int width, int height, const std::string &title, int fps = 60);
        ~Window();
    
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
    
        bool shouldClose() const;
    
        void beginFrame(Color background = DARKBLUE);
        void endFrame();
        void toggleFullscreen();
    
        int getWidth()  const { return GetScreenWidth();  }
        int getHeight() const { return GetScreenHeight(); }
};

}
