#pragma once

#include <ncurses.h>
#include <string.h>
#include "object.hpp"

class za_mapo{
public:
    WINDOW* win;
    object** objects;

    char* mapa;
    uint8_t rows;
    uint8_t length;

    za_mapo(uint8_t _rows, uint8_t _length, uint8_t pos_y, uint8_t pos_x);
    ~za_mapo();

    void init_palitra();
    void update_card();
    void redraw();
    void draw_range(object* target, int8_t range);
    void clear();

    void magnetic_search(object* from, object* to);
    void magnetic_search_neg(object* from, object* to);
    void indirect_moving(object* from);

    char move(object* obj, char direction);
    void free_move(object* target, char direction);

    object* get_object(object* obj, char direction);
};
