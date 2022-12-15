#pragma once

enum behavior_t{
    BHV_CHILL,
    BHV_WORRIED,
    BHV_ATTACK,
    BHV_PANIC,
    BHV_STUNNED,
    BHV_REST
};

enum action_t{
    ACT_STAND,
    ACT_ATTACK,
    ACT_ATTACK_NEARLEST_ENEMY,
    ACT_SEARCH_ENEMY,
    ACT_WALK,
    ACT_RUN_AVAY,
    ACT_REST
};

enum effect_t{
    PHYSIC_IMMUNITY,
    MAGIC_IMMUNITY,
    POISON_IMMUNITY,
    ACID_IMMUNITY,
    UNFROZENABLE,
    UNFLAMED,
    DIELECTRIC,

    MAGIC_ABSORBTION,
    POISON_ABSORBTION,
    ACID_ABSORBTION,
    ICE_ABSORBTION,
    FIRE_ABSORBTION,
    ELECTRIC_ABSORBTION,

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

extern const char* effect_names[51];

enum property_t{
    HEALTH,
    MANA,
    STRENGTH
};

extern const char* property_names[3];

enum fraction{
    HUMANITY,
    MONSTER
};

enum blink_t{
    HIDE,

    RED_STABILE,
    RED_ON,
    RED_OFF,
    RED_INVERT,
    RED_NO_INVERT,

    GREEN_STABILE,
    GREEN_ON,
    GREEN_OFF,
    GREEN_INVERT,
    GREEN_NO_INVERT
};

#define MAX_HEALTH 2000
#define MAX_MANA 200