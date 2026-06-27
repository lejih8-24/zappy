/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI player state
*/

#pragma once

#include "Game/Entity.hpp"
#include <game.hpp>

#include <string>

namespace GUI {

/*
** Related server commands:
** - pnw #n X Y O L N (new player)
** - ppo #n X Y O (player position)
** - plv #n L (player level)
** - pin #n X Y q0 q1 q2 q3 q4 q5 q6 (player inventory)
** - pex #n (player expulsion)
** - pbc #n M (player broadcast)
** - pfk #n (egg laying start)
** - pdr #n i (resource drop)
** - pgt #n i (resource collect)
** - pdi #n (player death)
*/
class Player : public Entity {
    public:
        enum class AnimState { Idle, Walk, Incantation, Dead, Broadcast, LayingEgg, LevelUp, Eject };

        struct DisplayPosition {
            float x = 0.0F;
            float y = 0.0F;
        };

        float rotationDeg = 0.0F;
        std::size_t level = 1;
        std::string teamName;
        Zappy::Game::Resources inventory = {};
        bool alive = true;
        AnimState animState = AnimState::Idle;
        float animStateStartTime = 0.0F;
        float animStateEndTime = 0.0f;
        DisplayPosition movementStart = {};
        DisplayPosition movementTarget = {};
        float movementStartTime = 0.0F;
        float movementEndTime = 0.0F;

        bool isMoving(float currentTime) const;
        void setAnimState(AnimState state, float currentTime, float duration = 0.0F);
        void snapDisplayPosition(int newX, int newY);
        void startMovement(float currentTime, DisplayPosition start, DisplayPosition target, float duration);
        DisplayPosition getDisplayPosition(float currentTime) const;
        AnimState getEffectiveAnimState(float currentTime) const;
        float getAnimationElapsed(float currentTime, AnimState effectiveState) const;

    public:
        Player(int id = 0, int x = 0, int y = 0, std::size_t level = 1, const std::string &teamName = "");
        ~Player() = default;
};

}
