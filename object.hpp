#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <map>
#include "object_defs.hpp"
#include "text_field.hpp"

using namespace std;

union effect{
    struct{
        int16_t time;
        int16_t amount;
    } timed;
    int32_t large;
};

class object{
protected:

    map<property_t, int32_t> propertyes;
    map<effect_t, effect> effects;
    string name;

public:
    static log* l;
    uint8_t X;
    uint8_t Y;

    fraction fract;

    void act(effect_t type, effect e);
    void calculate();

    virtual char get_type() = 0;
    effect* get_effect(effect_t type);
    const char* get_name();
    bool is_alive();
    fraction get_fraction();
    virtual void print_spells(menu* _menu) = 0;

    object(int8_t _X, int8_t _Y, string _name) : X(_X), Y(_Y), name(_name) {}
};