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
    bool is_permanent:1;
    bool is_shared:1;
    effect_t type:14;
};

bool operator<(const effect_def& a, const effect_def& b);

union effect{
    struct{
        int16_t is_long:1;
        int16_t time:15;
        int16_t amount;
    } timed;
    int32_t large;
};

enum effect_bhf{
    SHARMANENT,
    SHARED,
    PERMANENT,
    PURE
};

struct state{
    map<property_t, int32_t> propertyes;
    map<effect_def, effect> effects;

    static log* l;

    void act(effect_def type, effect e);
    void calculate();

    effect* get_effect(effect_def type);
    bool there_is_effect(effect_t type);
    bool there_is_effect(effect_def type);

    int32_t* get_property(property_t type);
    bool there_is_property(property_t type);

    bool request_property(property_t prop, size_t value);
};

effect_bhf get_effect_behavior(effect_def def);