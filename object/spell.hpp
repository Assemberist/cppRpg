#pragma once

#include <cstdint>
#include <cstddef>

struct spell{
    uint32_t level:7;
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