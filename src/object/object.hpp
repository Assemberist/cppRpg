#pragma once

#include "items.hpp"
#include "common/drawable_object.hpp"

#include "enums/action.hpp"
#include "enums/behavior.hpp"
#include "enums/fraction.hpp"

#define abs(A, B) ((A) - (B) > 0 ? (A) - (B) : (B) - (A))

struct expirience{
    size_t full;
    size_t avail;

    void add(size_t amount);
    bool request(size_t amount);
};

class object : public drawable_object, public logger_of_shit<object>{
public:
    state stat;

    expirience exp;

    vector<item> equipment;
    vector<item> inventory;

    // The code is crutch for execution act_throw() in user_ifc.
    item* item_to_use;

protected:
    map<spell_t, spell> spells;

    string name;

    behavior_t behavior;
    fraction_t fract;

public:
    object(int8_t _X, int8_t _Y, string _name) : drawable_object(_X, _Y), name(_name), behavior(BHV_ATTACK) {}

    const char* get_name();
    map<spell_t, spell>& get_spells();
    fraction_t get_fraction();

    virtual action_t turn() = 0;

    void pick_up_item(item& it);
    bool equip(vector<item>::iterator it);
    bool unequip(vector<item>::iterator it);
    void drop_item(vector<item>::iterator it);
    void put_item(vector<item>::iterator it, object* target);

    void act(effect_def, effect);
    void trigger(spell_trigger_t trigger, object* target);

    void put_spell(spell_t type, spell spell);
    void remove_spell(spell_t type);


    void set_behavior(behavior_t bhv);

    bool is_alive();
    bool check_enemy(object* target);
    spell_t choose_attack_spells(object* target);
    
    virtual ~object();
};