#pragma once

#include <ncurses.h>
#include <string.h>
#include "object.hpp"

class za_mapo{
public:
    WINDOW* win;
    drawable_object** objects;

    char* mapa;
    uint8_t rows;
    uint8_t length;

    za_mapo(uint8_t _rows, uint8_t _length, uint8_t pos_y, uint8_t pos_x);
    ~za_mapo();

    void init_palitra();
    void update_card();
    void redraw();
    void draw_range(drawable_object* target, int8_t range);
    void clear();

    void magnetic_search(drawable_object* from, drawable_object* to);
    void magnetic_search_neg(drawable_object* from, drawable_object* to);
    void indirect_moving(drawable_object* from);

    char move(drawable_object* obj, char direction);
    void free_move(drawable_object* target, char direction);

    drawable_object* get_object(drawable_object* obj, char direction);
};
