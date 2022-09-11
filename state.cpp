#include "state.hpp"

bool operator<(const effect_def& a, const effect_def& b){ return a.type < b.type; }

void state::calculate(){

}

bool state::request_property(property_t prop, size_t value){
    auto i = propertyes.find(prop);
    if(i == propertyes.end())
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

int32_t* state::get_property(property_t type){
    auto i = propertyes.find(type);
    return (i != propertyes.end() ? &i->second : NULL);
}

bool state::there_is_property(property_t type){
    return (propertyes.find(type) != propertyes.end());
}

bool state::there_is_effect(effect_t type){
    effect_def def;
    def.type = type;

    def.is_shared = false;
    def.is_permanent = false;
    if(effects.find(def) != effects.end()) return true;

    def.is_shared = true;
    if(effects.find(def) != effects.end()) return true;

    def.is_shared = false;
    def.is_permanent = true;
    if(effects.find(def) != effects.end()) return true;

    def.is_shared = true;
    return (effects.find(def) != effects.end());
}

bool state::there_is_effect(effect_def type){
    return (effects.find(type) != effects.end());
}

effect_bhf get_effect_behavior(effect_def def){
    return (def.is_permanent ?
        def.is_shared ? SHARMANENT : PERMANENT :
        def.is_shared ? SHARED : PURE);
}