/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** IClient interface
*/

#pragma once


namespace Zappy {
    using Tick = unsigned int;

    class IClient {
        public:
            virtual ~IClient() = default;

            virtual void update(Tick elapsedTicks) = 0;

            // Satisfy ClientSocketType constraint
            virtual short requiredEvents() const = 0;
            virtual void registerEvents(short revents) = 0;
            virtual int fileno() const = 0;
            virtual bool isOpen() const = 0;
    };
}
