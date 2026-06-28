/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Unit tests for GuiArgs
*/

#include <criterion/criterion.h>
#include "GuiArgs.hpp"
#include "exceptions/ArgsException.hpp"

#include <vector>

using GUI::GuiArgs;
using ArgsException = Zappy::Exceptions::ArgsException;

static GuiArgs parse(std::initializer_list<const char *> args)
{
    std::vector<const char *> argv = {"zappy_gui"};
    argv.insert(argv.end(), args.begin(), args.end());
    return GuiArgs::parseArgs(static_cast<int>(argv.size()), const_cast<char **>(argv.data()));
}

Test(gui_args, minimal_valid)
{
    auto args = parse({"-p", "4242"});
    cr_assert_eq(args.getPort(), 4242);
    cr_assert_str_eq(args.getHost().c_str(), "127.0.0.1");
    cr_assert_str_eq(args.getPack().c_str(), "default");
    cr_assert_eq(args.getWidth(), 1440);
    cr_assert_eq(args.getHeight(), 900);
}

Test(gui_args, all_args)
{
    auto args = parse({"-p", "1234", "-h", "myhost", "--pack", "penguin",
        "--width", "1920", "--height", "1080"});
    cr_assert_eq(args.getPort(), 1234);
    cr_assert_str_eq(args.getHost().c_str(), "myhost");
    cr_assert_str_eq(args.getPack().c_str(), "penguin");
    cr_assert_eq(args.getWidth(), 1920);
    cr_assert_eq(args.getHeight(), 1080);
}

Test(gui_args, missing_port)
{
    cr_assert_throw(parse({}), ArgsException);
}

Test(gui_args, port_zero)
{
    cr_assert_throw(parse({"-p", "0"}), ArgsException);
}

Test(gui_args, port_too_high)
{
    cr_assert_throw(parse({"-p", "65536"}), ArgsException);
}

Test(gui_args, port_not_a_number)
{
    cr_assert_throw(parse({"-p", "abc"}), ArgsException);
}

Test(gui_args, port_boundary_min)
{
    auto args = parse({"-p", "1"});
    cr_assert_eq(args.getPort(), 1);
}

Test(gui_args, port_boundary_max)
{
    auto args = parse({"-p", "65535"});
    cr_assert_eq(args.getPort(), 65535);
}

Test(gui_args, width_too_low)
{
    cr_assert_throw(parse({"-p", "4242", "--width", "100"}), ArgsException);
}

Test(gui_args, width_too_high)
{
    cr_assert_throw(parse({"-p", "4242", "--width", "8000"}), ArgsException);
}

Test(gui_args, height_too_low)
{
    cr_assert_throw(parse({"-p", "4242", "--height", "100"}), ArgsException);
}

Test(gui_args, height_too_high)
{
    cr_assert_throw(parse({"-p", "4242", "--height", "5000"}), ArgsException);
}

Test(gui_args, width_boundary_min)
{
    auto args = parse({"-p", "4242", "--width", "320"});
    cr_assert_eq(args.getWidth(), 320);
}

Test(gui_args, width_boundary_max)
{
    auto args = parse({"-p", "4242", "--width", "7680"});
    cr_assert_eq(args.getWidth(), 7680);
}

Test(gui_args, height_boundary_min)
{
    auto args = parse({"-p", "4242", "--height", "240"});
    cr_assert_eq(args.getHeight(), 240);
}

Test(gui_args, height_boundary_max)
{
    auto args = parse({"-p", "4242", "--height", "4320"});
    cr_assert_eq(args.getHeight(), 4320);
}

Test(gui_args, unknown_arg)
{
    cr_assert_throw(parse({"-p", "4242", "--unknown"}), ArgsException);
}

Test(gui_args, missing_value_for_p)
{
    cr_assert_throw(parse({"-p"}), ArgsException);
}

Test(gui_args, missing_value_for_width)
{
    cr_assert_throw(parse({"-p", "4242", "--width"}), ArgsException);
}

Test(gui_args, missing_value_for_height)
{
    cr_assert_throw(parse({"-p", "4242", "--height"}), ArgsException);
}

Test(gui_args, port_negative)
{
    cr_assert_throw(parse({"-p", "-1"}), ArgsException);
}

Test(gui_args, width_not_a_number)
{
    cr_assert_throw(parse({"-p", "4242", "--width", "abc"}), ArgsException);
}
