#pragma once

#include "state.hpp"

enum item_group{
    ARMORY_HEAD,
    ARMORY_HAND,
    ARMORY_FOOT,
    CLOTH_BODY,
    CLOTH_FOOT,
    AMULET,
    HOLDABLE_ONE_HAND,
    HOLDABLE_TWO_HAND,
    USEABLE_ITEM
};

enum item_t{
    WOOD_SHIELD,
    WOOD_SWORD,
    WOOD_BOW,

    IRON_SHIELD,
    IRON_SWORD,
    IRON_HUMMER,
    IRON_BOW,

    JACKET,
    JEANS,

    LATE_HELM,
    LATE_ARMORY,
    LATE_GLOVE,    
    LATE_FOOT,

    LATEX_HELM,
    LATEX_ARMORY,
    LATEX_GLOVE,    
    LATEX_FOOT,

    ICE_CRYSTALL,
    ACID_COLBA,
    STONE,
    HEALING_RUNE,
    SINTOL,

    NOTHING_ITEM
};

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
