/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** ArgumentParseException class
*/

#include "ArgumentParseException.hpp"
#include <sstream>


Zappy::Exceptions::ArgumentParseException::ArgumentParseException(std::string_view progName)
    : ParseException("")
    , m_Status(0)
{
    std::ostringstream msg;

    msg << "Usage: " << progName << " -p <port> -x <width> -y <height> -n <name> [<name>...] -c <clients> [-f <tickspeed>]\n"
        << "Run a Zappy server locally on the specified port.\n"
        << "Example: " << progName << " -p 5000 -x 10 -y 10 -n Team1 Team2 Team3 -c 10\n"
        << "\n"
        << "Available options:\n"
        << "  -p=PORT             Specifies the server's port\n"
        << "  -x=WIDTH            Sets the width of the map\n"
        << "  -y=HEIGHT           Sets the height of the map\n"
        << "  -n=NAME1[,NAME...]  Specifies the names of the available teams\n"
        << "  -c=MAXCLIENTS       Limits the max amount of clients per team\n"
        << "  -f=TICKSPEED        Sets the simulation's tick speed\n"
        << "  -w                  Optimizes the server to wait for updates\n"
        << "  -h                  Display this help message and exit\n";

    m_Message = msg.str();
}

Zappy::Exceptions::ArgumentParseException::ArgumentParseException(std::string_view progName, std::string_view msgShort)
    : ArgumentParseException(progName)
{
    m_Status = BaseException::status();
    m_Message = std::string(progName) + ": " + std::string(msgShort) + "\n" + m_Message;
}
