#pragma once

#include "state.hpp"

struct item{
    struct{
        item_group group:16;
        item_t type_name:16;
    } info;
    spell_slot* slots;
    state stat;
};

struct item_def{
    const char* text;
    vector<item>::iterator it;
};

extern const char* item_names[];

bool is_item_for_single_target(item_t thing);
