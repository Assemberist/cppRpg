#pragma once

#include "state.hpp"

enum item_group{
    ARMORY_HEAD,
    ARMORY_HAND,
    ARMORY_FOOT,
    CLOTH_BODY,
    CLOTH_FOOT,
    AMULET,
    WEAPON_COLD,
    WEAPON_RANGE,
    STUF,
    SINGLE_USE_ITEM
};

enum item_t{
    WOOD_SHIELD,
    WOOD_SWORD,
    WOOD_BOW,

    IRON_SHIELD,
    IRON_SWORD,
    IRON_BOW,

    JACKET,
    JEANS,

    LATE_HELM,
    LATE_ARMORY,
    LATE_GLOVE,    
    LATE_FOOT,

    LEATHER_HELM,
    LEATHER_ARMORY,
    LEATHER_GLOVE,    
    LEATHER_FOOT,

    NOTHING_ITEM
};

struct item{
    struct{
        item_group group:16;
        item_t type_name:16;
    } info;
    state stat;
};

struct item_def{
    const char* text;
    vector<item>::iterator it;
};

extern const char* item_names[];