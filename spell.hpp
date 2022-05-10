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