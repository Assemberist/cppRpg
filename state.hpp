#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <map>

#include "spell.hpp"
#include "object_defs.hpp"


#ifndef DONT_LOG_ACTIONS

#include "text_field.hpp"

#else
#ifdef DONT_LOG_STATE

#include "text_field.hpp"

#endif
#endif

using namespace std;

union effect{
    struct{
        int16_t time;
        int16_t amount;
    } timed;
    int32_t large;
};

struct state{
    map<property_t, int32_t> propertyes;
    map<effect_t, effect> effects;

#ifndef DONT_LOG_STATE
    static log* l;
#endif

    void act(effect_t type, effect e);
    void calculate();

    effect* get_effect(effect_t type);
    bool there_is_effect(effect_t type);

    int32_t* get_property(property_t type);
    bool there_is_property(property_t type);

    bool request_property(property_t prop, size_t value);
};