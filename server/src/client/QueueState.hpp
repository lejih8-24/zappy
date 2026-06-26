/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** QueueState class
*/

#pragma once

#include "BaseState.hpp"
#include <deque>


namespace Zappy::Client {
    template <std::size_t S>
    class QueueState : public BaseState {
        public:
            static constexpr std::size_t SEND_QUEUE_MAX_SIZE = S;

        private:
            std::deque<std::string> m_SendQueue;

        public:
            QueueState() noexcept : BaseState(), m_SendQueue() {}

        protected:
            inline void queueMessage(const std::string& message) { m_SendQueue.emplace_back(message); }
            inline void queueMessage(std::string&& message) { m_SendQueue.emplace_back(message); }
            inline std::string popMessages() noexcept { return mergeEvents(); }
            inline bool hasMessages() noexcept { return !m_SendQueue.empty(); }

        private:
            /**
             * Merges all* events into a single
             * string so it can be sent off to
             * the client in a single write() call
             * (*up to a limit of EVENT_MERGE_LIMIT).
             *
             * Assumes m_SendQueue.size() > 0.
             */
            std::string mergeEvents()
            {
                std::string events = std::move(m_SendQueue.front());
                m_SendQueue.pop_front();

                for (std::size_t i = 0; !m_SendQueue.empty() && i < SEND_QUEUE_MAX_SIZE; i++) {
                    events += m_SendQueue.front();
                    m_SendQueue.pop_front();
                }

                return events;
            }
    };
}
