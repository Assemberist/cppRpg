#pragma once

#include "state.hpp"
#include "spell.hpp"

#include "enums/item.hpp"
#include "enums/item_group.hpp"

struct item{
    item_group_t group;
    item_t type_name;
    spell_slot* slots;
    state stat;
};

bool is_item_for_single_target(item_t thing);
