#pragma once

#include "user_ifc_lib.hpp"

// to do 1: object target among all objects

struct screen{
    za_mapo* mapa;
    menu* common_menu;
    inventory* bag;
    inventory* loot;

    // we can remove log only if all debug level are disabled
    #if !defined(DONT_LOG_ACTIONS) || !defined(DONT_LOG_STATE)
    log* common_log;
    #endif
};

object* search_targets(object* obj, screen s, size_t range);

bool user_turn(object* u, screen s);
