/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Overlay panel displaying selected player info
*/

#include "UI/PlayerInfoPanel.hpp"
#include "Game/ResourceColors.hpp"
#include "UI/Scale.hpp"

#include "raylib.h"

#include <array>
#include <string>

static constexpr std::array<const char *, 7> RESOURCE_NAMES = {
    "Food", "Linemate", "Deraumere", "Sibur", "Mendiane", "Phiras", "Thystame"
};

static const char *animStateName(GUI::Player::AnimState state)
{
    switch (state) {
        case GUI::Player::AnimState::Idle:        return "Idle";
        case GUI::Player::AnimState::Walk:        return "Walk";
        case GUI::Player::AnimState::Incantation: return "Incantation";
        case GUI::Player::AnimState::Dead:        return "Dead";
        case GUI::Player::AnimState::Broadcast:   return "Broadcast";
        case GUI::Player::AnimState::LayingEgg:   return "Laying Egg";
        case GUI::Player::AnimState::LevelUp:     return "Level Up";
        case GUI::Player::AnimState::Eject:       return "Ejecting";
    }
    return "Unknown";
}

namespace GUI {

void PlayerInfoPanel::draw(const Player &player) const
{
    const int padding    = UI::scaleSize(12);
    const int fontSize   = UI::scaleSize(20);
    const int lineHeight = UI::scaleSize(26);
    const int margin     = UI::scaleSize(16);
    const int panelWidth = UI::scaleSize(260);
    const int panelHeight = padding * 2 + 5 * lineHeight + lineHeight / 2 + 7 * lineHeight;
    const int x = margin;
    const int y = GetScreenHeight() - panelHeight - margin;
    const int textX = x + padding;

    DrawRectangle(x, y, panelWidth, panelHeight, Fade(BLACK, 0.70F));
    DrawRectangleLines(x, y, panelWidth, panelHeight, DARKGRAY);

    int textY = y + padding;

    DrawText(("Player #" + std::to_string(player.id)).c_str(), textX, textY, fontSize, YELLOW);
    textY += lineHeight;

    DrawText((player.teamName + "  L" + std::to_string(player.level)).c_str(), textX, textY, fontSize, RAYWHITE);
    textY += lineHeight;

    DrawText(("Pos: (" + std::to_string(player.x) + ", " + std::to_string(player.y) + ")").c_str(),
        textX, textY, fontSize, LIGHTGRAY);
    textY += lineHeight;

    DrawText(("Dir: " + std::to_string(static_cast<int>(player.rotationDeg)) + " deg").c_str(),
        textX, textY, fontSize, LIGHTGRAY);
    textY += lineHeight;

    DrawText((std::string("State: ") + animStateName(player.getEffectiveAnimState(GetTime()))).c_str(),
        textX, textY, fontSize, GREEN);
    textY += lineHeight;

    DrawLine(textX, textY + lineHeight / 4, x + panelWidth - padding, textY + lineHeight / 4, DARKGRAY);
    textY += lineHeight / 2;

    std::size_t i = 0;
    for (unsigned int qty : player.inventory) {
        DrawText((std::string(RESOURCE_NAMES[i]) + ": " + std::to_string(qty)).c_str(),
            textX, textY, fontSize, ResourceColors[i]);
        textY += lineHeight;
        ++i;
    }
}

}
