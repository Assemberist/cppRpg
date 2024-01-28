#pragma once

#include "state.hpp"
#include "spell.hpp"

#include "enums/item.hpp"
#include "enums/item_group.hpp"

struct item{
    struct{
        item_group_t group:16;
        item_t type_name:16;
    } info;
    spell_slot* slots;
    state stat;
};

struct item_def{
    const char* text;
    vector<item>::iterator it;
};

bool is_item_for_single_target(item_t thing);
