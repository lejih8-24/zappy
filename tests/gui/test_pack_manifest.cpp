/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Unit tests for PackManifest JSON parsing
*/

#include <criterion/criterion.h>
#include "Theme/PackManifest.hpp"

using GUI::PackManifest;

Test(pack_manifest, missing_pack_returns_defaults)
{
    PackManifest m("nonexistent_pack_xyz");
    cr_assert_float_eq(m.number("playerScale", 1.0f), 1.0f, 1e-5f);
}

Test(pack_manifest, number_player_scale)
{
    PackManifest m("penguin");
    cr_assert_float_eq(m.number("playerScale", 1.0f), 0.5f, 1e-5f);
}

Test(pack_manifest, number_player_label_height)
{
    PackManifest m("penguin");
    cr_assert_float_eq(m.number("playerLabelHeight", 2.5f), 2.0f, 1e-5f);
}

Test(pack_manifest, number_missing_key_returns_default)
{
    PackManifest m("penguin");
    cr_assert_float_eq(m.number("nonexistentKey", 42.0f), 42.0f, 1e-5f);
}

Test(pack_manifest, number_clamped_to_min)
{
    PackManifest m("penguin");
    cr_assert_float_eq(m.number("playerScale", 1.0f, 0.8f, 100.0f), 0.8f, 1e-5f);
}

Test(pack_manifest, rotation_player_rotation_y)
{
    PackManifest m("penguin");
    auto rot = m.rotation("playerRotation");
    cr_assert_float_eq(rot.y, 180.0f, 1e-5f);
    cr_assert_float_eq(rot.x, 0.0f, 1e-5f);
    cr_assert_float_eq(rot.z, 0.0f, 1e-5f);
}

Test(pack_manifest, rotation_missing_block_returns_zero)
{
    PackManifest m("penguin");
    auto rot = m.rotation("noSuchBlock");
    cr_assert_float_eq(rot.x, 0.0f, 1e-5f);
    cr_assert_float_eq(rot.y, 0.0f, 1e-5f);
    cr_assert_float_eq(rot.z, 0.0f, 1e-5f);
}

Test(pack_manifest, background_color)
{
    PackManifest m("penguin");
    auto color = m.backgroundColor();
    cr_assert_eq(color.r, 180);
    cr_assert_eq(color.g, 180);
    cr_assert_eq(color.b, 255);
}

Test(pack_manifest, animations_map)
{
    PackManifest m("penguin");
    auto anims = m.animations();
    cr_assert(!anims.empty());
    cr_assert(anims.count("walk") > 0);
    cr_assert_eq(anims.at("walk"), 6);
    cr_assert(anims.count("dead") > 0);
    cr_assert_eq(anims.at("dead"), 1);
    cr_assert(anims.count("broadcast") > 0);
    cr_assert_eq(anims.at("broadcast"), 0);
}

Test(pack_manifest, animations_missing_returns_empty)
{
    PackManifest m("nonexistent_pack_xyz");
    auto anims = m.animations();
    cr_assert(anims.empty());
}
