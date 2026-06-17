/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** ZipIterator utility class
*/

#pragma once

#include <utility>


namespace Zappy::Utils {
    template <typename T>
    concept IteratorType = requires (T& iterA, T& iterB)
    {
        { ++iterA };
        { *iterA };
        { iterA == iterB };
        { iterA != iterB };
    };

    /**
     * ZipIterator class.
     *
     * Creates an iterator by zipping
     * two existing iterators together.
     *
     * Example:
     * std::array<int, 5> ids;
     * std::array<std::string_view, 5> names;
     *
     * ZipIterator end(ids.end(), names.end());
     *
     * for (ZipIterator it(ids.begin(), names.begin()); it != end; ++it) {
     *     auto [id, name] = *it;
     *     std::cout << id << ": " << name << std::endl;
     * }
     */
    template <IteratorType A, IteratorType B>
    class ZipIterator {
        A m_IteratorA;
        B m_IteratorB;

        using ValueA = decltype(*m_IteratorA);
        using ValueB = decltype(*m_IteratorB);
        using ValuePair = std::pair<ValueA, ValueB>;

        public:
            ZipIterator(A iterator);

            ZipIterator& operator++()
            {
                ++m_IteratorA;
                ++m_IteratorB;
                return *this;
            }

            ValuePair operator*() const
            {
                return { *m_IteratorA, *m_IteratorB };
            }

            inline bool operator==(const ZipIterator& other) const noexcept { return !(*this != other); }
            bool operator!=(const ZipIterator& other) const noexcept
            {
                return
                    (m_IteratorA != other.m_IteratorA) ||
                    (m_IteratorB != other.m_IteratorB);
            }
    };
}
