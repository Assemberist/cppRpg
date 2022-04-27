#pragma once

#include "object.hpp"
#include <ncurses.h>
#include <stdint.h>
#include <string.h>

enum blink_t{
    HIDE,

    RED_STABILE,
    RED_ON,
    RED_OFF,
    RED_INVERT,
    RED_NO_INVERT,

    GREEN_STABILE,
    GREEN_ON,
    GREEN_OFF,
    GREEN_INVERT,
    GREEN_NO_INVERT
};

struct blink_cfg{
    object* o;
    struct{
        blink_t is_hide:4;
    } cfg;
};

class za_mapo{
public:
    WINDOW* win;
    blink_cfg* objects;

    char* mapa;
    size_t rows;
    size_t length;

    za_mapo(size_t _rows, size_t _length, size_t pos_y, size_t pos_x);
    ~za_mapo();

    void init_palitra();
    void update_card();
    void clear();

    void magnetic_search(object* from, object* to);
    void indirect_moving(object* from);

    char move(object* obj, char direction);
};

struct screen{
    za_mapo* mapa;
    log* common_log;
    menu* common_menu;
};
