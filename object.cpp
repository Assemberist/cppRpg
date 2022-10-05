#include "object.hpp"

bool in_range(object* user, object* target, size_t range){
    return (abs(user->X, target->X) <= range && abs(user->Y, target->Y) <= range ? true : false);
}

const char* object::get_name(){ return name.c_str(); }
fraction object::get_fraction(){ return fract; }
map<spell_t, spell>& object::get_spells(){ return spells; }

bool object::is_alive(){ return !stat.there_is_effect(DEAD); }
void object::set_behavior(behavior_t bhv){ behavior = bhv; }

void object::put_spell (spell_t type, spell sp) { spells.insert({type, sp}); }
void object::remove_spell(spell_t type){ spells.erase(type); }

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
        size_t i;
        for(i = equipment.size(); i; i--){
            if(!e.timed.amount) break;

            effect part = {
                e.timed.time >> 1,
                rand() % e.timed.amount
            };

            e.timed.amount -= part.timed.amount;
            equipment[i].stat.act(def, part);
        }

        for(; i; i--)
            equipment[i].stat.act(def, {e.timed.time >> 1, 0});
    }
    stat.act(def, e);

    for(size_t i = equipment.size(); i--;)
        collect_effects(this, &equipment[i].stat, def);
}

bool object::equip(vector<item>::iterator it){
    // find correct slot
    for(int j=equipment.size(); j--;)
        if( equipment[j].info.group == it->info.group &&
            equipment[j].info.type_name == NOTHING_ITEM){
                equipment[j] = *it;
                inventory.erase(it);
                for(auto i = it->stat.effects.begin(); i != it->stat.effects.end(); i++){
                    if(i->first.is_permanent && i->first.is_shared){
                        auto eff = stat.effects.find(i->first);
                        if(eff == stat.effects.end()) eff->second.large += i->second.large;
                        // permanent shared effects should have time = 1 and is_long = 0
                        else stat.effects.insert(*i);
                    }
                }
                return true;
            }

    return false;
}

bool object::unequip(vector<item>::iterator it){
    // here can be check of possibilty to unequip item.
    for(auto i = it->stat.effects.begin(); i != it->stat.effects.end(); i++){
        if(i->first.is_permanent && i->first.is_shared){
            auto eff = stat.effects.find(i->first);
            if(eff->second.timed.time > 1) eff->second.large -= i->second.large;
            else stat.effects.erase(eff);
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

void object::use_item(vector<item>::iterator it){
    // usable utems should not have shared effects.
    for(auto i = it->stat.effects.begin(); i != it->stat.effects.end(); i++){
        if(i->second.timed.time > 0 && !i->first.is_permanent){
            stat.act(i->first, i->second);
            i->second.timed.time--;
        }
    }
}

void object::collect_effects(object* obj, state* stat, effect_def group){
    for(auto i = stat->effects.begin(); i != stat->effects.end(); i++)
        if(i->first.is_shared && !i->first.is_permanent)
            obj->stat.act(i->first, i->second);
}