/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** ArgumentParseException class
*/

#include "ArgumentParseException.hpp"
#include <sstream>


Zappy::Exceptions::ArgumentParseException::ArgumentParseException(std::string_view progName, std::string_view msgShort)
   : ParseException("")
{
   std::ostringstream msg;

   msg << progName << ": " << msgShort << '\n'
      << "Usage: " << progName << " -p <port> -x <width> -y <height> -n <name> [<name>...] -c <clients> [-f <tickspeed>]\n"
      << "Run a Zappy server locally on the specified port.\n"
      << "Example: " << progName << " -p 5000 -x 10 -y 10 -n Team1 Team2 Team3 -c 10\n"
      << '\n'
      << "Available options:\n"
      << "  -p=PORT             Specifies the server's port\n"
      << "  -x=WIDTH            Sets the width of the map\n"
      << "  -y=HEIGHT           Sets the height of the map\n"
      << "  -n=NAME1[,NAME...]  Specifies the names of the available teams\n"
      << "  -c=MAXCLIENTS       Limits the max amount of clients per team\n"
      << "  -f=TICKSPEED        Sets the simulation's tick speed\n";

   m_Message = msg.str();
}
