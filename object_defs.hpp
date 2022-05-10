#pragma once

enum effect_t{
    PHYSIC_IMMUNITY,
    MAGIC_IMMUNITY,
    POISON_IMMUNITY,
    ACID_IMMUNITY,
    UNFROZENABLE,
    UNFLAMED,
    DIELECTRIC,

    PHYSIC_PROTECT,
    MAGIC_PROTECT,
    POISON_PROTECT,
    ACID_PROTECT,
    FROZING_PROTECT,
    FIRE_PROTECT,
    ELECTRIC_PROTECT,

    PHYSIC_WEAKNESS,
    MAGIC_WEAKNESS,
    POISON_WEAKNESS,
    ACID_WEAKNESS,
    FROZING_WEAKNESS,
    FIRE_WEAKNESS,
    ELECTRIC_WEAKNESS,

    FLYING,
    CORROSION,
    SHOCKED,
    FIRING,
    WET,
    STUNNED,
    FRIZED,
    SPLITTED_OPEN,
    DEAD,
    REGENERATION,
    POISONED,
    MANA_REGENERATION,

    ACID_ATTACK,
    CRUSH_ATTACK,
    SLASH_ATTACK,
    COLUSH_ATTACK,
    MAGIC_ATTACK,

    HEAL,
    POISON_DAMAGE,
    MANA_RESTORE,
    FIRE_DAMAGE,
    ELECTRIC_DAMAGE,
    ICE_DAMAGE
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