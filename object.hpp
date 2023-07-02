#pragma once

#include "items.hpp"

#define abs(A, B) ((A) - (B) > 0 ? (A) - (B) : (B) - (A))

struct expirience{
    size_t full;
    size_t avail;

    void add(size_t amount);
    bool request(size_t amount);
};

// Move the class to separate file
class drawable_object{
public:
    uint8_t X;
    uint8_t Y;

    blink_t graph_state;

    drawable_object(uint8_t _X, uint8_t _Y) : X(_X), Y(_Y) {}
    virtual char get_type() = 0;

    // card.cpp uses this but it is violation.
    // it should be removed after it will be fixed.
    virtual bool is_alive() = 0;
};

class object : public drawable_object{
public:
    state stat;

    expirience exp;

    static log* l;

    vector<item> equipment;
    vector<item> inventory;

    // The code is crutch for execution act_throw() in user_ifc.
    item* item_to_use;

protected:
    map<spell_t, spell> spells;

    string name;

    behavior_t behavior;
    fraction fract;

    struct target_l{
        object* target;
        target_l* next;
    } targets;

public:
    object(int8_t _X, int8_t _Y, string _name) : drawable_object(_X, _Y), name(_name), behavior(BHV_ATTACK) {}

    const char* get_name();
    map<spell_t, spell>& get_spells();
    fraction get_fraction();

    virtual action_t turn() = 0;

    void pick_up_item(item& it);
    bool equip(vector<item>::iterator it);
    bool unequip(vector<item>::iterator it);
    void drop_item(vector<item>::iterator it);
    void put_item(vector<item>::iterator it, object* target);

    void act(effect_def, effect);
    void trigger(spell_trigger trigger, object* target);

    void put_spell(spell_t type, spell spell);
    void remove_spell(spell_t type);


    void set_behavior(behavior_t bhv);

    bool is_alive();
    bool check_enemy(object* target);
    spell_t choose_attack_spells(object* target);
    
    virtual ~object();
};