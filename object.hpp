#pragma once

#include "state.hpp"

#define abs(A, B) ((A) - (B) > 0 ? (A) - (B) : (B) - (A))

class object{
protected:
    vector<spell*> spells;

    string name;
    behavior_t behavior;

    fraction fract;

public:
    state stat;

    static log* l;
    uint8_t X;
    uint8_t Y;

    blink_t graph_state:4;

    object(int8_t _X, int8_t _Y, string _name) : X(_X), Y(_Y), name(_name), behavior(BHV_ATTACK) {}

    virtual char get_type() = 0;
    const char* get_name();
    vector<spell*>& get_spells();
    fraction get_fraction();

    virtual action_t turn() = 0;

    void put_spell(spell* sp);
    void remove_spell(spell* sp);

    void set_behavior(behavior_t bhv);

    bool is_alive();
    bool check_enemy(object* target);
    spell_t choose_attack_spells(object* target);
};