/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Server class
*/

#pragma once

#include <zappy/client.hpp>
#include <zappy/game.hpp>
#include <zappy/exceptions.hpp>
#include <lattice/networking.hpp>
#include <charconv>
#include <string>
#include <chrono>
#include <deque>


namespace Zappy {
    using LatticeServer = Lattice::Server<Client::Client, Lattice::NonBlockingSocket, 50, true>;

    class Server : public LatticeServer {
        using Client = Client::Client;

        static constexpr std::chrono::milliseconds DEFAULT_TIMEOUT { 20'000 };

        Game::Game m_Game;
        std::chrono::milliseconds m_UpdateDuration;
        std::chrono::steady_clock::time_point m_UpdateStart;
        std::chrono::nanoseconds m_DeltaTime;
        std::deque<std::string> m_EventQueue;

        public:
            Server(std::string_view ip, std::uint16_t port);
            Server(Server&& server);

            void pushEvent(std::string&& evt) { m_EventQueue.push_front(std::move(evt)); }
            std::optional<std::string> popEvent();

            inline void operator=(Server&& other) { swap(other); }
            void swap(Server& other)
            {
                std::swap(m_Game, other.m_Game);
                std::swap(m_UpdateStart, other.m_UpdateStart);
                std::swap(m_DeltaTime, other.m_DeltaTime);
                std::swap(m_EventQueue, other.m_EventQueue);
                LatticeServer::swap(other);
            }

        private:
            void onStart() override;
            void onShutdown() override;
            void updateStart() override;

            void updateClient(Client& client) override;
            void onClientAccepted(Client& client) override;
            void onClientDisconnected(Client& client) override;

            int pollTimeout(int previousTimeout) override;

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

                    inline Builder&& fromArguments(int argc, char *argv[]) { return fromArguments(std::span(const_cast<const char**>(argv), const_cast<const char**>(argv + argc))); }
                    Builder&& fromArguments(std::span<const char*> args);

                    inline Builder&& setHostname(std::string_view hostname)                { m_Hostname = hostname;          return std::move(*this); }
                    inline Builder&& setPort(std::uint16_t port)                           { m_Port = port;                  return std::move(*this); }
                    inline Builder&& setMapSize(std::uint32_t width, std::uint32_t height) { m_MapSize = { width, height };  return std::move(*this); }
                    inline Builder&& setMaxClientsPerTeam(std::uint32_t clients)           { m_ClientsPerTeam = clients;     return std::move(*this); }
                    Builder&& setTickSpeed(std::uint32_t speed);
                    Builder&& addTeamName(std::string_view name);
                    Builder&& setTeamNames(std::span<std::string_view> names);
                    Builder&& setTeamNames(std::span<const char*> names);

                    Server build() &&;

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
