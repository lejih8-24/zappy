/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GLB model theme - renders players via CharacterModel
*/

#include "Theme/GlbTheme.hpp"

#include <cmath>

static constexpr int WALK_ANIM_INDEX = 2;
static constexpr float ANIM_FPS = 24.0f;

namespace GUI {

GlbTheme::GlbTheme() = default;

void GlbTheme::drawTile(Vector3 pos, Vector3 size, bool isLight) const
{
    _fallback.drawTile(pos, size, isLight);
}

void GlbTheme::drawResource(std::size_t resourceIndex, Vector3 pos, float height) const
{
    _fallback.drawResource(resourceIndex, pos, height);
}

void GlbTheme::drawPlayer(Vector3 pos, float rotationDeg) const
{
    float frame = std::fmod(GetTime() * ANIM_FPS, _character.getAnimationFrameCount(WALK_ANIM_INDEX));
    _character.draw(pos, rotationDeg, WALK_ANIM_INDEX, frame);
}

void GlbTheme::drawEgg(Vector3 pos) const
{
    _fallback.drawEgg(pos);
}

}
