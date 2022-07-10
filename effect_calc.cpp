#include "object.hpp"

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
            
            if((i = effects.find(PHYSIC_WEAKNESS)) != effects.end()){
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
                    sprintf(buff, "%s got %4d of physic damage.\n", name.c_str(), e.timed.amount);
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

            if((i = effects.find(MAGIC_WEAKNESS)) != effects.end()){
                object::l->newline("Object have weakness before magic.\n");
                e.timed.amount += i->second.timed.amount;
            }

            if(j != propertyes.end()){
                char buff[1024];
                const char* format;
                if(j->second <= e.timed.amount){
                    format = "This attack was letal for the %s.\n";
                    j->second = 0;
                    effects.insert({DEAD, {0,0}});
                }
                else{
                    format = "%s received %d of magic damage.\n";
                    j->second -= e.timed.amount;
                }
                sprintf(buff, format, name.c_str(), e.timed.amount);
                object::l->newline(buff);
            }
            else{
                object::l->newline("It can not be killed or broken.\n");
                return;
            }
            break;
        }

        case ELECTRIC_DAMAGE:{
            if(effects.find(DEAD) != effects.end()){
                object::l->newline("Defibrilation had not effect.\n");
                return;
            }

            if(effects.find(DIELECTRIC) != effects.end()){
                object::l->newline("It is dielectric.\n");
                return;
            }

            auto i = effects.find(ELECTRIC_PROTECT);
            auto j = propertyes.find(HEALTH);

            if(i != effects.end()){
                object::l->newline("Smh dielectric decreased damage.\n");
                if(i->second.timed.amount >= e.timed.amount){
                    object::l->newline("Dielectric neutralise all damage.\n");
                    return;
                }
                e.timed.amount -= i->second.timed.amount;
            }

            if((i = effects.find(ELECTRIC_WEAKNESS)) != effects.end()){
                object::l->newline("Object have weakness before electric.\n");
                e.timed.amount += i->second.timed.amount;
            }

            if((i = effects.find(WET)) != effects.end()){
                object::l->newline("Wet objects have weakeness before electric.\n");
                e.timed.amount *= 1.5;
            }

            if(j != propertyes.end()){
                char buff[1024];
                const char* format;
                if(j->second <= e.timed.amount){
                    format = "This attack was letal for the %s.\n";
                    j->second = 0;
                    effects.insert({DEAD, {0,0}});
                }
                else{
                    format = "%s received %d of electric damage.\n";
                    j->second -= e.timed.amount;
                }
                sprintf(buff, format, name.c_str(), e.timed.amount);
                object::l->newline(buff);
            }
            else{
                object::l->newline("It can not be killed or broken.\n");
                return;
            }
            break;
        }

        case FIRE_DAMAGE:{
            if(effects.find(DEAD) != effects.end()){
                object::l->newline("The fire will clean this object. Or not.\n");
                return;
            }

            if(effects.find(UNFLAMED) != effects.end()){
                object::l->newline("It can not burns.\n");
                return;
            }

            auto i = effects.find(FIRE_PROTECT);
            auto j = propertyes.find(HEALTH);

            if(i != effects.end()){
                object::l->newline("Fire goes weaker.\n");
                if(i->second.timed.amount >= e.timed.amount){
                    object::l->newline("Fire disappired.\n");
                    return;
                }
                e.timed.amount -= i->second.timed.amount;
            }

            if((i = effects.find(WET)) != effects.end()){
                object::l->newline("Wet objects whorse fireing.\n");
                e.timed.amount /= 2;
            }
            
            if((i = effects.find(FIRE_WEAKNESS)) != effects.end()){
                object::l->newline("Object is burning very good.\n");
                e.timed.amount += i->second.timed.amount;
            }


            if(j != propertyes.end()){
                char buff[1024];
                const char* format;
                if(j->second <= e.timed.amount){
                    format = "The %s was burned.\n";
                    j->second = 0;
                    effects.insert({DEAD, {0,0}});
                }
                else{
                    format = "%s received %d of fire damage.\n";
                    j->second -= e.timed.amount;
                }
                sprintf(buff, format, name.c_str(), e.timed.amount);
                object::l->newline(buff);
            }
            else{
                object::l->newline("It can not be burned.\n");
                return;
            }
            break;
        }

        default:
            object::l->newline("!! Logic is not realised yet !!\n");
    }
}
