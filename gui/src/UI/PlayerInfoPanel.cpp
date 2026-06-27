/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Overlay panel displaying selected player info
*/

#include "UI/PlayerInfoPanel.hpp"
#include "Game/ResourceColors.hpp"

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

void PlayerInfoPanel::draw(const Canvas &canvas, const Player &player, float currentTime) const
{
    const int padding = canvas.scaleSize(12);
    const int fontSize = canvas.scaleSize(20);
    const int lineHeight = canvas.scaleSize(26);
    const int margin = canvas.scaleSize(16);
    const int panelWidth = canvas.scaleSize(260);
    const int panelHeight = padding * 2 + 5 * lineHeight + lineHeight / 2 + 7 * lineHeight;
    const int x = margin;
    const int y = canvas.height() - panelHeight - margin;
    const int textX = x + padding;

    canvas.drawRectangle(x, y, panelWidth, panelHeight, canvas.fade(Colors::Black, 0.70F));
    canvas.drawRectangleLines(x, y, panelWidth, panelHeight, Colors::DarkGray);

    int textY = y + padding;

    canvas.drawText("Player #" + std::to_string(player.id), textX, textY, fontSize, Colors::Yellow);
    textY += lineHeight;

    canvas.drawText(player.teamName + "  L" + std::to_string(player.level), textX, textY, fontSize,
        Colors::RayWhite);
    textY += lineHeight;

    canvas.drawText("Pos: (" + std::to_string(player.x) + ", " + std::to_string(player.y) + ")",
        textX, textY, fontSize, Colors::LightGray);
    textY += lineHeight;

    canvas.drawText("Dir: " + std::to_string(static_cast<int>(player.rotationDeg)) + " deg",
        textX, textY, fontSize, Colors::LightGray);
    textY += lineHeight;

    canvas.drawText(std::string("State: ") + animStateName(player.getEffectiveAnimState(currentTime)),
        textX, textY, fontSize, Colors::Green);
    textY += lineHeight;

    canvas.drawLine(textX, textY + lineHeight / 4, x + panelWidth - padding, textY + lineHeight / 4,
        Colors::DarkGray);
    textY += lineHeight / 2;

    std::size_t i = 0;
    for (unsigned int qty : player.inventory) {
        canvas.drawText(std::string(RESOURCE_NAMES[i]) + ": " + std::to_string(qty),
            textX, textY, fontSize, ResourceColors[i]);
        textY += lineHeight;
        ++i;
    }
}

}
