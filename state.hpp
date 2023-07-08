#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <map>

#include "spell.hpp"
#include "material.h"
#include "object_defs.hpp"
#include "text_field.hpp"

using namespace std;

struct effect_def{
    bool is_shared:1;
    effect_t type:15;
};

bool operator<(const effect_def& a, const effect_def& b);

struct effect{
    int16_t time;
    int16_t amount;
};

enum effect_bhf{
    SHARMANENT,
    SHARED,
    PERMANENT,
    PURE
};

struct flag_reg{
    uint64_t is_dead:1;
};

struct state{
    map<effect_def, int16_t> effects_perm;
    map<effect_def, effect> effects;

    static log* l;

    flag_reg flags;

    void act(effect_def type, effect e);
    void calculate();

    effect* get_effect(effect_def type);
    bool there_is_effect(effect_t type);
    bool there_is_effect(effect_def type);
    bool is_there_effect_perm(effect_t type);

    int16_t* get_effect_perm(effect_def type);
    bool there_is_property(effect_def type);

    bool request_property(effect_def prop, int16_t value);
};

bool is_shared(effect_def def);