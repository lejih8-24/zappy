/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI player state
*/

#include "Game/Player.hpp"

#include <algorithm>

namespace GUI {

Player::Player(int id, int x, int y, std::size_t level, const std::string &teamName)
    : Entity(id, x, y)
    , level(level)
    , teamName(teamName)
{
    snapDisplayPosition(x, y);
}

bool Player::isMoving(float currentTime) const
{
    return movementEndTime > movementStartTime && currentTime < movementEndTime;
}

void Player::setAnimState(AnimState state, float currentTime, float duration)
{
    animState = state;
    animStateStartTime = currentTime;
    animStateEndTime = duration > 0.0F ? currentTime + duration : 0.0F;
}

void Player::snapDisplayPosition(int newX, int newY)
{
    DisplayPosition position = {static_cast<float>(newX), static_cast<float>(newY)};

    movementStart = position;
    movementTarget = position;
    movementStartTime = 0.0F;
    movementEndTime = 0.0F;
}

void Player::startMovement(float currentTime, DisplayPosition start, DisplayPosition target, float duration)
{
    movementStart = start;
    movementTarget = target;
    movementStartTime = currentTime;
    movementEndTime = currentTime + duration;
}

Player::DisplayPosition Player::getDisplayPosition(float currentTime) const
{
    if (!isMoving(currentTime))
        return {static_cast<float>(x), static_cast<float>(y)};

    float duration = movementEndTime - movementStartTime;
    if (duration <= 0.0F)
        return {static_cast<float>(x), static_cast<float>(y)};

    float progress = std::clamp((currentTime - movementStartTime) / duration, 0.0F, 1.0F);
    return {
        movementStart.x + (movementTarget.x - movementStart.x) * progress,
        movementStart.y + (movementTarget.y - movementStart.y) * progress,
    };
}

Player::AnimState Player::getEffectiveAnimState(float currentTime) const
{
    if ((animState == AnimState::Idle || animState == AnimState::Walk)
        || (animStateEndTime > 0.0F && currentTime > animStateEndTime)) {
        if (isMoving(currentTime))
            return AnimState::Walk;
        return AnimState::Idle;
    }
    return animState;
}

float Player::getAnimationElapsed(float currentTime, AnimState effectiveState) const
{
    if (effectiveState == AnimState::Walk && isMoving(currentTime))
        return currentTime - movementStartTime;
    return currentTime - animStateStartTime;
}

}
