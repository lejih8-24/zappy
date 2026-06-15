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

namespace GUI {

class GuiArgs {
    public:
        static GuiArgs parseArgs(int argc, char **argv);
        static void printUsage(std::ostream &stream);

        const std::string &getHost() const;
        int getPort() const;
        void connect() const;

    private:
        static int parsePort(const std::string &value);
        static std::string requireValue(int argc, char **argv, int &index);

        std::string _host = "localhost";
        int _port = -1;
};

}
