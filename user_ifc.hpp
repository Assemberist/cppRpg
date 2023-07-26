#pragma once

#include "user_ifc_lib.hpp"

// to do 1: object target among all objects

struct screen{
    za_mapo* mapa;
    text_log* common_log;

    screen(char* card, drawable_object** objs);
    ~screen();
};

object* search_targets(object* obj, screen s, size_t range);

bool user_turn(object* u, screen s);
void setup_user_ifc();