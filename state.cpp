#include "state.hpp"

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

effect* state::get_effect(effect_t type){
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
    return (effects.find(type) != effects.end());
}
