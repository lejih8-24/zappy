/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** IState interface
*/

#pragma once

#include <type_traits>


namespace Zappy::Client {
    using Tick = unsigned int;

    class IState {
        public:
            virtual ~IState() = default;

            virtual void update(Tick elapsedTicks) = 0;
            // virtual void registerEvent() = 0;
    };

    template <typename T>
    concept StateType = std::is_base_of_v<IState, T>;
}
