#pragma once

#include "visual/curses/battle_scene.hpp"
#include <stdlib.h>
#include <vector>

using namespace std;

enum event_t{
    GROUP_MEMBER_CHANGED,
    GROUP_MEMBER_ATTACED,
    GROUP_MEMBER_KILLED,
    GROUP_DISAPPEAR,

    ENEMY_APPEARE,
    ENEMY_HIDE,
    ENEMY_SURRENDER,
    ENEMY_DEAD
};

class object_group {
    vector<object*> objects;
    fraction_t fract;
    
public:
    void add_object(object* obj){ objects.push_back(obj); }
    void del_object(object* obj){}
    void set_fraction(fraction_t f){ fract = f; }

    void handle_event(event_t type, ...){}
};

void init_graphic();
bool game_loop(object** objs, object* gamer, screen& s);