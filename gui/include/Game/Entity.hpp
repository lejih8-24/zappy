/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI identified state
*/

#pragma once

namespace GUI {

struct Position {
    int x = 0;
    int y = 0;
};

class Entity : public Position {
    public:
        Entity(int id = 0, int x = 0, int y = 0);
        ~Entity() = default;

        int getId() const { return _id; }
        void setId(int id) { _id = id; }

    private:
        int _id = 0;
};

}
