#pragma once

enum spell_t{
    FIREBALL,
    PUNCH,
    LIGHTING
};

struct spell{
    const char* name;
    spell_t type;
};

extern spell __spell_def[];

#define _SPELL_FIREBALL (__spell_def + 0)
#define _SPELL_PUNCH    (__spell_def + 1)
#define _SPELL_LIGTHING (__spell_def + 2)
