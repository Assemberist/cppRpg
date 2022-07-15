#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <map>

#include "spell.hpp"
#include "object_defs.hpp"
#include "text_field.hpp"

#define abs(A, B) ((A) - (B) > 0 ? (A) - (B) : (B) - (A))

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
    vector<spell*> spells;

    string name;
    behavior_t behavior;

    fraction fract;

public:
    static log* l;
    uint8_t X;
    uint8_t Y;

    blink_t graph_state:4;

    object(int8_t _X, int8_t _Y, string _name) : X(_X), Y(_Y), name(_name), behavior(BHV_ATTACK) {}

    virtual char get_type() = 0;
    const char* get_name();
    vector<spell*>& get_spells();

    void act(effect_t type, effect e);
    virtual action_t turn() = 0;
    void calculate();

    effect* get_effect(effect_t type);
    int32_t* get_property(property_t type);
    bool request_property(property_t prop, size_t value);

    void put_spell(spell* sp);
    void remove_spell(spell* sp);

    fraction get_fraction();

    void set_behavior(behavior_t bhv);

    bool is_alive();
    bool check_enemy(object* target);
    spell_t choose_attack_spells(object* target);
};