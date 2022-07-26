#include "object.hpp"

bool in_range(object* user, object* target, size_t range){
    return (abs(user->X, target->X) <= range && abs(user->Y, target->Y) <= range ? true : false);
}

const char* object::get_name(){ return name.c_str(); }
fraction object::get_fraction(){ return fract; }
vector<spell*>& object::get_spells(){ return spells; }

bool object::is_alive(){ return !stat.there_is_effect(DEAD); }
void object::set_behavior(behavior_t bhv){ behavior = bhv; }
void object::put_spell (spell* sp) { spells.push_back(sp); }

void object::remove_spell(spell* sp){
    for(auto i = spells.begin(); i != spells.end(); i++)
        if(*i == sp)
            spells.erase(i);
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

spell_t object::choose_attack_spells(object* target){
    for(auto i=spells.begin(); i != spells.end(); i++){
        switch((*i)->type){
            case FIREBALL:
                if(in_range(this, target, 5)){
                    return FIREBALL;
                }
                break;

            case PUNCH:
                if(in_range(this, target, 1)){
                    return PUNCH;
                }
                break;

            case LIGHTING:
                if(in_range(this, target, 5)){
                    return LIGHTING;
                }
                break;

            default:
                break;
        }
    }

    return NOTHING;
}
