#pragma once

#include "object.hpp"
#include "text_field.hpp"

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
    void redraw();
    void draw_range(object* target, int8_t range);
    void clear();

    void magnetic_search(object* from, object* to);
    void magnetic_search_neg(object* from, object* to);
    void indirect_moving(object* from);

    char move(object* obj, char direction);
    void free_move(object* target, char direction);
};

struct screen{
    za_mapo* mapa;
    menu* common_menu;
    inventory* bag;

    // we can remove log only if all debug level are disabled
    #if !defined(DONT_LOG_ACTIONS) || !defined(DONT_LOG_STATE)
    log* common_log;
    #endif
};

object* search_targets(object* obj, screen s, size_t range);
