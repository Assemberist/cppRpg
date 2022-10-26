#pragma once

#include <cstdint>

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

extern const char* spell_names[];