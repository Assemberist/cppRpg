#pragma once

#include <cstdint>

enum spell_t{
    FIREBALL,
    PUNCH,
    LIGHTING,
    NOTHING
};

struct spell{
    uint32_t level:7;
};

extern const char* spell_names[];
