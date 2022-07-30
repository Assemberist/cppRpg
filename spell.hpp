#pragma once

#include <cstdint>

enum spell_t{
    FIREBALL,
    PUNCH,
    LIGHTING,
    NOTHING
};

struct spell_def{
    const char* name;
    spell_t type;
};

struct spell{
    spell_def* definition;
    uint32_t level:7;
};

extern spell_def __spell_def[];

#define _SPELL_FIREBALL (__spell_def + 0)
#define _SPELL_PUNCH    (__spell_def + 1)
#define _SPELL_LIGTHING (__spell_def + 2)
#define _SPELL_NOTHING  (__spell_def + 3)