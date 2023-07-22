#include "object.hpp"
#include <cstdint>

bool in_range(object* user, object* target, size_t range){
    return (abs(user->X, target->X) <= range && abs(user->Y, target->Y) <= range ? true : false);
}

const char* object::get_name(){ return name.c_str(); }
fraction object::get_fraction(){ return fract; }
map<spell_t, spell>& object::get_spells(){ return spells; }

bool object::is_alive(){ return !stat.flags.is_dead; }
void object::set_behavior(behavior_t bhv){ behavior = bhv; }

void object::put_spell (spell_t type, spell sp) { spells.insert({type, sp}); }
void object::remove_spell(spell_t type){ spells.erase(type); }

size_t object::get_class(){ return proffession; }
void object::set_class(size_t cl){ proffession = cl; }

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

    return true;
}

spell_t object::choose_attack_spells(object* target){
    for(auto i=spells.begin(); i != spells.end(); i++){
        switch(i->first){
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

    return NOTHING_SPELL;
}

void object::trigger(spell_trigger trigger, object* target){
    // send triggers to equipment.
}

void expirience::add(size_t amount){
    full += amount;
    avail += amount;
}

bool expirience::request(size_t amount){
    if(avail < amount) return false;
    else{
        avail -= amount;
        return true;
    }
}

void object::act(effect_def def, effect e){
    if(def.is_shared){
        def.is_shared = false;

        size_t i;
        for(i = equipment.size(); i--;){
            if(equipment[i].info.type_name == NOTHING_ITEM)
                continue;

            effect part = {
                static_cast<int16_t>(e.time >> 1),
                static_cast<int16_t>(rand() % e.amount)
            };

            e.amount -= part.amount;
            equipment[i].stat.act(def, part);

            if(!e.amount) {
                while(i--)
                    if(equipment[i].info.type_name != NOTHING_ITEM)
                        equipment[i].stat.act(def, {static_cast<int16_t>(e.time >> 1), 0});

                break;
            }
        }
    }
    stat.act(def, e);
}

bool object::equip(vector<item>::iterator it){
    // find correct slot
    for(int j=equipment.size(); j--;)
        if( equipment[j].info.group == it->info.group &&
            equipment[j].info.type_name == NOTHING_ITEM){
                equipment[j] = *it;
                inventory.erase(it);
                for(auto i = it->stat.effects_perm.begin(); i != it->stat.effects_perm.end(); i++){
                    if(i->first.is_shared){
                        auto eff = stat.effects_perm.find(i->first);
                        if(eff == stat.effects_perm.end()) eff->second += i->second;
                        // permanent shared effects should have time = 1 and is_long = 0
                        else stat.effects_perm.insert(*i);
                    }
                }
                return true;
            }

    return false;
}

bool object::unequip(vector<item>::iterator it){
    // here can be check of possibilty to unequip item.
    for(auto i = it->stat.effects_perm.begin(); i != it->stat.effects_perm.end(); i++){
        if(i->first.is_shared){
            auto eff = stat.effects_perm.find(i->first);
            eff->second -= i->second;
        }
    }
    inventory.end() - 1;
    inventory.push_back(*it);
    it->info.type_name = NOTHING_ITEM;
    return true;
}

void object::pick_up_item(item& it){ inventory.push_back(it); }
void object::drop_item(vector<item>::iterator it){ inventory.erase(it); }

void object::put_item(vector<item>::iterator it, object* target){
    target->inventory.push_back(*it);
    inventory.erase(it);
}

object::~object(){}