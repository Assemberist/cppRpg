#include "state.hpp"

bool operator<(const effect_def& a, const effect_def& b){ return a.type < b.type; }

void state::calculate(){

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

    def.is_shared = false;
    if(effects.find(def) != effects.end()) return true;

    def.is_shared = true;
    return (effects.find(def) != effects.end());
}

bool state::is_there_effect_perm(effect_t type){
    effect_def def;
    def.type = type;

    def.is_shared = false;
    if(effects_perm.find(def) != effects_perm.end()) return true;

    def.is_shared = true;
    return (effects_perm.find(def) != effects_perm.end());
}

bool state::there_is_effect(effect_def type){
    return (effects.find(type) != effects.end());
}

bool is_shared(effect_def def){ return def.is_shared; }