/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI CLI arguments
*/

#pragma once

#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace GUI {

class GuiArgs {
    public:
        static GuiArgs parseArgs(int argc, char **argv);
        static void printUsage(std::ostream &stream);

        const std::string &getHost() const;
        int getPort() const;
        const std::string &getPack() const;
        void connect() const;

    private:
        static int parsePort(std::string_view value);
        static std::string_view requireValue(int argc, char **argv, int &index);

        std::string _host = "127.0.0.1";
        int _port = -1;
        std::string _pack = "green_man";
};

}
