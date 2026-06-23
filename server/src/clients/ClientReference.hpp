/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** ClientReference class
*/

#pragma once

#include "IClient.hpp"
#include <memory>


namespace Zappy {
    class ClientReference : public IClient {
        using PointerType = std::unique_ptr<IClient>;

        PointerType m_Reference;

        public:
            ClientReference(PointerType&& ref) : m_Reference(std::move(ref)) {}
            ClientReference(IClient* ref) : m_Reference(ref) {}
            ClientReference() : m_Reference() {}

            inline PointerType& ref() { return m_Reference; }
            inline const PointerType& ref() const { return m_Reference; }

            inline void update(Tick elapsedTicks) override { m_Reference->update(elapsedTicks); }

            inline short requiredEvents() const override { return m_Reference->requiredEvents(); }
            inline void registerEvents(short revents) override { return m_Reference->registerEvents(revents); }
            inline int fileno() const override { return m_Reference->fileno(); }
    };
}
