#pragma once

#include <cstdint>
#include <cstddef>

#include "enums/slot_type.hpp"
#include "enums/spell_direction.hpp"
#include "enums/spell_trigger.hpp"
#include "enums/spell.hpp"

struct spell{
    uint32_t level:7;
};

struct spell_slot_config{
    slot_type_t type;
    spell_direction_t direction;
    spell_trigger_t trigger;
    size_t charges;
};

struct spell_slot{
    spell_t type;
    spell_slot_config config;
    spell_slot* next;
};