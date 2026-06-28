/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Unit tests for Player animation state machine
*/

#include <criterion/criterion.h>
#include "Game/Player.hpp"

using GUI::Player;

Test(player, initial_state_is_idle)
{
    Player p(1, 3, 4, 1, "team1");
    cr_assert_eq(p.getEffectiveAnimState(0.0f), Player::AnimState::Idle);
}

Test(player, not_moving_initially)
{
    Player p(1, 0, 0, 1, "team1");
    cr_assert(!p.isMoving(0.0f));
}

Test(player, display_position_when_static)
{
    Player p(1, 3, 4, 1, "team1");
    auto pos = p.getDisplayPosition(0.0f);
    cr_assert_float_eq(pos.x, 3.0f, 1e-5f);
    cr_assert_float_eq(pos.y, 4.0f, 1e-5f);
}

Test(player, walk_state_while_moving)
{
    Player p(1, 0, 0, 1, "team1");
    p.startMovement(0.0f, {0.0f, 0.0f}, {1.0f, 0.0f}, 2.0f);
    cr_assert_eq(p.getEffectiveAnimState(1.0f), Player::AnimState::Walk);
}

Test(player, is_moving_during_movement)
{
    Player p(1, 0, 0, 1, "team1");
    p.startMovement(0.0f, {0.0f, 0.0f}, {1.0f, 0.0f}, 2.0f);
    cr_assert(p.isMoving(1.0f));
}

Test(player, not_moving_after_movement_ends)
{
    Player p(1, 0, 0, 1, "team1");
    p.startMovement(0.0f, {0.0f, 0.0f}, {1.0f, 0.0f}, 1.0f);
    cr_assert(!p.isMoving(5.0f));
}

Test(player, idle_after_movement_ends)
{
    Player p(1, 1, 0, 1, "team1");
    p.startMovement(0.0f, {0.0f, 0.0f}, {1.0f, 0.0f}, 1.0f);
    cr_assert_eq(p.getEffectiveAnimState(5.0f), Player::AnimState::Idle);
}

Test(player, display_position_interpolates_midway)
{
    Player p(1, 2, 0, 1, "team1");
    p.startMovement(0.0f, {0.0f, 0.0f}, {2.0f, 0.0f}, 2.0f);
    auto pos = p.getDisplayPosition(1.0f);
    cr_assert_float_eq(pos.x, 1.0f, 1e-5f);
    cr_assert_float_eq(pos.y, 0.0f, 1e-5f);
}

Test(player, display_position_at_start_of_movement)
{
    Player p(1, 2, 0, 1, "team1");
    p.startMovement(0.0f, {0.0f, 0.0f}, {2.0f, 0.0f}, 2.0f);
    auto pos = p.getDisplayPosition(0.0f);
    cr_assert_float_eq(pos.x, 0.0f, 1e-5f);
}

Test(player, timed_anim_overrides_walk)
{
    Player p(1, 0, 0, 1, "team1");
    p.startMovement(0.0f, {0.0f, 0.0f}, {1.0f, 0.0f}, 10.0f);
    p.setAnimState(Player::AnimState::Incantation, 0.0f, 5.0f);
    cr_assert_eq(p.getEffectiveAnimState(1.0f), Player::AnimState::Incantation);
}

Test(player, expired_anim_falls_back_to_walk)
{
    Player p(1, 0, 0, 1, "team1");
    p.startMovement(0.0f, {0.0f, 0.0f}, {1.0f, 0.0f}, 10.0f);
    p.setAnimState(Player::AnimState::Incantation, 0.0f, 1.0f);
    cr_assert_eq(p.getEffectiveAnimState(3.0f), Player::AnimState::Walk);
}

Test(player, snap_stops_movement)
{
    Player p(1, 0, 0, 1, "team1");
    p.startMovement(0.0f, {0.0f, 0.0f}, {1.0f, 0.0f}, 5.0f);
    p.snapDisplayPosition(1, 0);
    cr_assert(!p.isMoving(1.0f));
    cr_assert_eq(p.getEffectiveAnimState(1.0f), Player::AnimState::Idle);
}

Test(player, snap_cancels_in_progress_movement)
{
    Player p(1, 0, 0, 1, "team1");
    p.startMovement(0.0f, {0.0f, 0.0f}, {5.0f, 0.0f}, 10.0f);
    cr_assert(p.isMoving(1.0f));
    p.snapDisplayPosition(5, 3);
    cr_assert(!p.isMoving(1.0f));
}

Test(player, animation_elapsed_uses_movement_start_when_walking)
{
    Player p(1, 1, 0, 1, "team1");
    p.startMovement(2.0f, {0.0f, 0.0f}, {1.0f, 0.0f}, 5.0f);
    float elapsed = p.getAnimationElapsed(4.0f, Player::AnimState::Walk);
    cr_assert_float_eq(elapsed, 2.0f, 1e-5f);
}

Test(player, animation_elapsed_uses_anim_start_when_idle)
{
    Player p(1, 0, 0, 1, "team1");
    p.setAnimState(Player::AnimState::Broadcast, 1.0f, 3.0f);
    float elapsed = p.getAnimationElapsed(2.5f, Player::AnimState::Broadcast);
    cr_assert_float_eq(elapsed, 1.5f, 1e-5f);
}
