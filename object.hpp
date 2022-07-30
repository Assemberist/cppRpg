#pragma once

#include "state.hpp"

#define abs(A, B) ((A) - (B) > 0 ? (A) - (B) : (B) - (A))

struct expirience{
    size_t full;
    size_t avail;

    void add(size_t amount);
    bool request(size_t amount);
};

class object{
protected:
    map<spell_t, spell> spells;

    string name;

    behavior_t behavior;

    fraction fract;

public:
    state stat;

    expirience exp;

#ifndef DONT_LOG_ACTIONS
    static log* l;
#endif
    
    uint8_t X;
    uint8_t Y;

    blink_t graph_state:4;

    object(int8_t _X, int8_t _Y, string _name) : X(_X), Y(_Y), name(_name), behavior(BHV_ATTACK) {}

    virtual char get_type() = 0;
    const char* get_name();
    map<spell_t, spell>& get_spells();
    fraction get_fraction();

    virtual action_t turn() = 0;

    void put_spell(spell_t type, spell spell);
    void remove_spell(spell_t type);

    void set_behavior(behavior_t bhv);

    bool is_alive();
    bool check_enemy(object* target);
    spell_t choose_attack_spells(object* target);
};