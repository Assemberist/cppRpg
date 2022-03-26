#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include "object_defs.hpp"

using namespace std;

struct property{
    property_t property;
    int32_t value;
};

struct effect{
    effect_t type;
    union{
        struct{
            int16_t time;
            int16_t amount;
        };
        int32_t large;
    } value;
};

class object{
protected:
    vector<property> propertyes;
    vector<effect> effects;
    string name;
public:
    uint8_t X;
    uint8_t Y;

    void act(effect e);
    void calculate();

    virtual char get_type() = 0;

    object(int8_t _X, int8_t _Y, string _name) : X(_X), Y(_Y), name(_name) {}
};