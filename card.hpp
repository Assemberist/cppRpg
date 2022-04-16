#pragma once

#include "object.hpp"
#include <ncurses.h>
#include <stdint.h>
#include <string.h>

class za_mapo{
public:
    WINDOW* win;
    object** objects;

    char* mapa;
    size_t rows;
    size_t length;

    za_mapo(size_t _rows, size_t _length, size_t pos_y, size_t pos_x);
    ~za_mapo();

    void init_palitra();
    void update_card();

    void magnetic_search(object* from, object* to);
    void indirect_moving(object* from);

    char move(object* obj, char direction);
};
