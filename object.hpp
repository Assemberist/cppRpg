#pragma once

#include <vector>
#include <cstdint>
#include <string>

using namespace std;

enum effect_t{
    FLYING,
    ELECTRIC_DAMAGE,
    SHOCKED,
    FIRE_DAMAGE,
    FIRING,
    DRY,
    STUNNED,
    FRIZED,
    ICE_DAMAGE,
    DEAD,
    REGENERATION,
    POISONED,
    CRUSH_ATTACK,
    SLASH_ATTACK,
    COLUSH_ATTACK,
};

enum property_t{
    HEALTH,
    MANA
};

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