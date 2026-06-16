/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Server class
*/

#pragma once

#include <lattice/networking.hpp>
#include <zappy/exceptions.hpp>
#include <charconv>


namespace Zappy {
    class Server : public Lattice::Server<Lattice::CachingSocket<>> {
        using Client = Lattice::CachingSocket<>;

        public:
            Server(std::string_view ip, std::uint16_t port);
            Server(Server&& server);
            ~Server();

        private:
            void onClientAccepted(const Client& client) override;
            void onClientDisconnected(const Client& client) override;
            void updateClient(Client& client) override;

        public:
            class Builder {
                std::string_view m_Hostname;
                std::uint16_t m_Port;
                std::pair<std::uint32_t, std::uint32_t> m_MapSize;
                std::uint32_t m_TickSpeed;
                std::uint32_t m_ClientsPerTeam;
                std::vector<std::string> m_TeamNames;

                public:
                    Builder();

                    inline Builder& fromArguments(int argc, char *argv[]) { return fromArguments(std::span(const_cast<const char**>(argv), const_cast<const char**>(argv + argc))); }
                    Builder& fromArguments(std::span<const char*> args);

                    inline Builder& setHostname(std::string_view hostname)                { m_Hostname = hostname;          return *this; }
                    inline Builder& setPort(std::uint16_t port)                           { m_Port = port;                  return *this; }
                    inline Builder& setMapSize(std::uint32_t width, std::uint32_t height) { m_MapSize = { width, height };  return *this; }
                    inline Builder& setTickSpeed(std::uint32_t speed)                     { m_TickSpeed = speed;            return *this; }
                    inline Builder& setMaxClientsPerTeam(std::uint32_t clients)           { m_ClientsPerTeam = clients;     return *this; }
                    inline Builder& addTeamName(std::string_view name)                    { m_TeamNames.emplace_back(name); return *this; }
                    Builder& setTeamNames(std::span<std::string_view> names);
                    Builder& setTeamNames(std::span<const char*> names);

                    Server build();

                private:
                    std::span<const char*> parseOption(std::string_view progName, std::span<const char*> args);

                    std::span<const char*> parsePortOption(std::string_view progName, std::span<const char*> args);
                    std::span<const char*> parseMapWidthOption(std::string_view progName, std::span<const char*> args);
                    std::span<const char*> parseMapHeightOption(std::string_view progName, std::span<const char*> args);
                    std::span<const char*> parseTeamNamesOption(std::string_view progName, std::span<const char*> args);
                    std::span<const char*> parseTickSpeedOption(std::string_view progName, std::span<const char*> args);
                    std::span<const char*> parseMaxClientsOption(std::string_view progName, std::span<const char*> args);

                    template <std::integral T>
                    void toInt(std::string_view repr, T& value)
                    {
                        auto [ptr, ec] = std::from_chars(repr.begin(), repr.end(), value);

                        if (ec == std::errc())
                            return;

                        if (ec == std::errc::result_out_of_range)
                            throw Exceptions::ParseException("value " + std::string(repr) + " is out of range");

                        throw Exceptions::ParseException("value " + std::string(repr) + " is not a number");
                    }
            };
    };
}
