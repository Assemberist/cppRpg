#include "object.hpp"
#include "text_field.hpp"

void object::act(effect_t type, effect e){
    switch(type){
        case CRUSH_ATTACK:{
            if(effects.find(DEAD) != effects.end()){
                object::l->newline("Stop punch the dead body!\n");
                return;
            }

            if(effects.find(PHYSIC_IMMUNITY) != effects.end()){
                object::l->newline("Physic attacks had no effect.\n");
                return;
            }

            auto i = effects.find(PHYSIC_PROTECT);
            auto j = propertyes.find(HEALTH);
            
            if(i != effects.end()){
                object::l->newline("Attacked object have physic resistance.\n");
                if(i->second.timed.amount > e.timed.amount){
                    object::l->newline("The armory too strong to deal a damage.\n");
                    return;
                }
                e.timed.amount -= i->second.timed.amount;
            }
            else if((i = effects.find(PHYSIC_WEAKNESS)) != effects.end()){
                object::l->newline("Attacked object have physic weakness.\n");
                e.timed.amount += i->second.timed.amount;
            }

            if(j != propertyes.end()){
                if((i = effects.find(FRIZED)) != effects.end()){
                    if(j->second < e.timed.amount / 2){
                        object::l->newline("It was splitted open.\n");
                        effects.insert({DEAD, {0, 0}});
                        return;
                    }
                }
                
                if(j->second > e.timed.amount){
                    char buff[1024];
                    sprintf(buff, "%s got %4d damage.\n", name.c_str(), e.timed.amount);
                    j->second -= e.timed.amount;
                    object::l->newline(buff);
                }
                else{
                    char buff[1024];
                    sprintf(buff, "%s was killed.\n", name.c_str());
                    object::l->newline(buff);
                    effects.insert({DEAD, {0,0}});
                }
            }
            else{
                object::l->newline("It is can not be killed or broken.\n");
                return;
            }
            break;
        }

        case MAGIC_ATTACK:{
            if(effects.find(DEAD) != effects.end()){
                object::l->newline("Attakc magic will not make he alive.\n");
                return;
            }

            if(effects.find(MAGIC_IMMUNITY) != effects.end()){
                object::l->newline("Magic attack was not effect.\n");
                return;
            }

            auto i = effects.find(MAGIC_PROTECT);
            auto j = propertyes.find(HEALTH);

            if(i != effects.end()){
                object::l->newline("Antimagic aura decrease power of the magic.\n");
                if(i->second.timed.amount >= e.timed.amount){
                    object::l->newline("The magic was destroyed.\n");
                    return;
                }
                e.timed.amount -= i->second.timed.amount;
            }
            else if((i = effects.find(MAGIC_WEAKNESS)) != effects.end()){
                object::l->newline("Object have weakness before magic.\n");
                e.timed.amount += i->second.timed.amount;
            }

            if(j != propertyes.end()){
                char buff[1024];
                const char* format = j->second <= e.timed.amount ?
                    "This attack was letal for the %s.\n" :
                    "%s received %d damage.\n";
                sprintf(buff, format, name.c_str(), e.timed.amount);
            }
            else{
                object::l->newline("It can not be killed or broken.\n");
                return;
            }
            break;
        }

        default:
            object::l->newline("!! Logic is not realised yet !!");
    }
}

effect* object::get_effect(effect_t type){
    auto i = effects.find(type);
    return (i != effects.end() ? &i->second : NULL);
}

const char* object::get_name(){ return name.c_str(); }
fraction object::get_fraction(){ return fract; }
bool object::is_alive(){ return effects.find(DEAD) == effects.end(); }

void object::calculate(){

}