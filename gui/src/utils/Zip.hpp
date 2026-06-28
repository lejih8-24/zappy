/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** zip utility class
*/

#pragma once

#include "ZipIterator.hpp"


namespace Zappy::Utils {
    template <typename T>
    concept IterableType = requires (const T& value)
    {
        { value.begin() } -> IteratorType;
        { value.end() }   -> IteratorType;
    };

    /**
     * zip class.
     *
     * Zips two iterable types together
     * to create by creating a single
     * iterable type that yields the value
     * of both values at once.
     *
     * Example:
     * std::array<int, 5> ids;
     * std::array<std::string_view, 5> names;
     *
     * for (auto [id, name] : zip(ids, names)) {
     *     std::cout << id << ": " << name << std::endl;
     * }
     *
     * Note:
     * The reason this class is in
     * lowercase is that it's meant to
     * be used as a function more so than
     * a type.
     */
    template <IterableType A, IterableType B>
    class zip {
        A& m_ValueA;
        B& m_ValueB;

        using IteratorA = decltype(m_ValueA.begin());
        using IteratorB = decltype(m_ValueB.begin());

        public:
            zip(A& valueA, B& valueB) : m_ValueA(valueA), m_ValueB(valueB) {}

            ZipIterator<IteratorA, IteratorB> begin() { return ZipIterator(m_ValueA.begin(), m_ValueB.begin()); }
            ZipIterator<IteratorA, IteratorB> end()   { return ZipIterator(m_ValueA.end(), m_ValueB.end());     }
    };
}
