#include "object.hpp"

bool in_range(object* user, object* target, size_t range){
    return (abs(user->X, target->X) <= range && abs(user->Y, target->Y) <= range ? true : false);
}

effect* object::get_effect(effect_t type){
    auto i = effects.find(type);
    return (i != effects.end() ? &i->second : NULL);
}

int32_t* object::get_property(property_t type){
    auto i = propertyes.find(type);
    return (i != propertyes.end() ? &i->second : NULL);
}

const char* object::get_name(){ return name.c_str(); }
fraction object::get_fraction(){ return fract; }
bool object::is_alive(){ return effects.find(DEAD) == effects.end(); }
void object::set_behavior(behavior_t bhv){ behavior = bhv; }
void object::put_spell (spell* sp) { spells.push_back(sp); }

void object::remove_spell(spell* sp){
    for(auto i = spells.begin(); i != spells.end(); i++)
        if(*i == sp)
            spells.erase(i);
}

bool object::buy_property(property_t prop, size_t value){
    auto i = propertyes.find(prop);
    if(i == propertyes.end())
        return false;

    if(i->second >= value){
        i->second -= value;
        return true;
    }

    return false;
}

void object::print_spells(menu* _menu){
    _menu->set_content((menu_element**)(&spells[0]), spells.size());
}

bool object::check_enemy(object* target){
    switch(fract){
        case HUMANITY:
            switch(target->get_fraction()){
                case MONSTER:
                    return true;
                
                default:
                    return false;
            }

        case MONSTER:
            switch(target->get_fraction()){
                case MONSTER:
                    return false;
                
                default:
                    return true;
            }
    }
}

bool object::use_attack_spells(object* target){
    for(auto i=spells.begin(); i != spells.end(); i++){
        switch((*i)->type){
            case FIREBALL:
                if(in_range(this, target, 5)){
                    act_fireball(this, target);
                    return true;
                }
                break;

            case PUNCH:
                if(in_range(this, target, 1)){
                    act_punch(this, target);
                    return true;
                }
                break;

            case LIGHTING:
                if(in_range(this, target, 5)){
                    act_lighting(this, target);
                    return true;
                }
                break;

            default:
                break;
        }
    }

    return false;
}

void object::calculate(){

}

void act_punch(object* obj, object* target){
    auto* value = obj->get_property(STRENGTH);
    if(value){
        target->act(CRUSH_ATTACK, {0, *value});
    }
    else{
        char arr[50];
        sprintf(arr, "%s can't punch.\n", obj->get_name());
        object::l->newline(arr);
    }
}

void act_lighting(object* obj, object* target){
    if(obj->buy_property(MANA, 10)){
        target->act(ELECTRIC_DAMAGE, {0, 40});
        target->act(MAGIC_ATTACK, {0, 10});
    }
    else{
        object::l->newline("Not enough mana.\n");
    }
}

// todo
void act_fireball(object* obj, object* target){
    
}