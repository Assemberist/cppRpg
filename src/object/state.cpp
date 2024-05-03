#include "state.hpp"

bool operator<(const effect_def& a, const effect_def& b){ return a.type < b.type; }

void state::calculate(){
    for(auto i = effects.begin(); i != effects.end(); i++){
        if(i->first.is_active())
            act({0, i->first.type}, {0, i->second.amount});

        if(i->second.time == 0) effects.erase(i);
        else i->second.time--;
    }

    for(auto i = effects_perm.begin(); i != effects_perm.end(); i++)
        if(i->first.is_active())
            act({0, i->first.type}, {0, i->second.amount});
}

bool state::request_property(effect_def prop, int16_t value){
    auto i = effects_perm.find(prop);
    if(i == effects_perm.end())
        return false;

    if(i->second >= value){
        i->second -= value;
        return true;
    }

    return false;
}

effect* state::get_effect(effect_def type){
    auto i = effects.find(type);
    return (i != effects.end() ? &i->second : NULL);
}

int16_t* state::get_effect_perm(effect_def type){
    auto i = effects_perm.find(type);
    return (i != effects_perm.end() ? &i->second : NULL);
}

bool state::there_is_effect(effect_t type){
    effect_def def;
    def.type = type;

    def.shared = false;
    if(effects.find(def) != effects.end()) return true;

    def.shared = true;
    return (effects.find(def) != effects.end());
}

bool state::is_there_effect_perm(effect_t type){
    effect_def def;
    def.type = type;

    def.shared = false;
    if(effects_perm.find(def) != effects_perm.end()) return true;

    def.shared = true;
    return (effects_perm.find(def) != effects_perm.end());
}

bool state::there_is_effect(effect_def type){
    return (effects.find(type) != effects.end());
}

bool effect_def::is_shared(){ return shared; }
bool effect_def::is_active(){ return type >= FIRING; }