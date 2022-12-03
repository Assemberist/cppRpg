#pragma once

#include <cstdint>
#include <cstddef>

enum spell_t{
    FIREBALL,
    PUNCH,
    LIGHTING,
    NOTHING_SPELL,
    THROW
};

struct spell{
    uint32_t level:7;
};

enum spell_direction{
    OWNER,
    NEAR_ENEMY,
    NEAR_FRIEND,
    OWNER_TARGET
};

enum spell_trigger{
    EVERY_TURN,
    BY_OWNER,
    BEING_ATTACKED,
    ANY_EFFECT
};

enum slot_type{
    CHARGE,
    BATTERY,
    ENCHANCEMENT
};

struct spell_slot_config{
    slot_type type;
    spell_direction direction;
    spell_trigger trigger;
    size_t charges;
};

struct spell_slot{
    spell_t type;
    spell_slot_config config;
    spell_slot* next;
};

extern const char* spell_names[];