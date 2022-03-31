#pragma once

enum effect_t{
    FLYING,
    CORROSION,
    SHOCKED,
    FIRING,
    DRY,
    STUNNED,
    FRIZED,
    DEAD,
    REGENERATION,
    POISONED,
    MANA_REGENERATION,

    ACID_ATTACK,
    CRUSH_ATTACK,
    SLASH_ATTACK,
    COLUSH_ATTACK,

    HEAL,
    POISON_DAMAGE,
    MANA_RESTORE,
    FIRE_DAMAGE,
    ELECTRIC_DAMAGE,
    ICE_DAMAGE,
};

enum property_t{
    HEALTH,
    MANA
};

enum fraction{
    HUMANITY,
    MONSTER
};

#define MAX_HEALTH 2000
#define MAX_MANA 200