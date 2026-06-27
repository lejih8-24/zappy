/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI render
*/

#pragma once

#include "Camera.hpp"
#include "Game/GameState.hpp"
#include "Map.hpp"
#include "Theme/ThemeManager.hpp"
#include "UI/Hud.hpp"
#include "UI/PlayerInfoPanel.hpp"
#include "Window.hpp"

#include <optional>
#include <string_view>

namespace Zappy::Networking {
    class GraphicsClient;
}

namespace GUI {

class Render {
    public:
        Render(std::string_view host, int port, std::string_view pack = "default");
        void renderLoop(Zappy::Networking::GraphicsClient &client);

    private:
        void update(Zappy::Networking::GraphicsClient &client);
        void pollServerEvents(Zappy::Networking::GraphicsClient &client);
        void handleGameInput();
        void drawFrame();
        void draw3DScene();
        void drawCrosshair() const;
        void drawFocusOverlay() const;
        void drawCameraLockLabel() const;
        void drawHelpText() const;
        void drawSelectedPlayerPanel() const;

        GameState _state;
        Window _window;
        ThemeManager _themeManager;
        Map _map;
        GameCamera _camera;
        Hud _hud;
        PlayerInfoPanel _playerInfoPanel;
        std::optional<int> _selectedPlayerId;
};

}
