#include "state.hpp"

#define MAX_LINES 10

static const char* lines[10];

void state::act(effect_t type, effect e){
    size_t num = 0;

    switch(type){
        case CRUSH_ATTACK:{
            if(effects.find(DEAD) != effects.end()){
                state::l->newline("Stop punch the dead body!\n");
                return;
            }

            if(effects.find(PHYSIC_IMMUNITY) != effects.end()){
                state::l->newline("Physic attacks had no effect.\n");
                return;
            }

            auto i = effects.find(PHYSIC_PROTECT);
            auto j = propertyes.find(HEALTH);
            
            if(i != effects.end()){
                state::l->newline("Attacked object have physic resistance.\n");
                if(i->second.timed.amount > e.timed.amount){
                    state::l->newline("The armory too strong to deal a damage.\n");
                    return;
                }
                e.timed.amount -= i->second.timed.amount;
            }
            
            if((i = effects.find(PHYSIC_WEAKNESS)) != effects.end()){
                state::l->newline("Attacked object have physic weakness.\n");
                e.timed.amount += i->second.timed.amount;
            }

            if(j != propertyes.end()){
                if((i = effects.find(FRIZED)) != effects.end()){
                    if(j->second < e.timed.amount / 2){
                        state::l->newline("It was splitted open.\n");
                        effects.insert({DEAD, {0, 0}});
                        return;
                    }
                }
                
                if(j->second > e.timed.amount){
                    char buff[1024];
                    sprintf(buff, "Received %4d of physic damage.\n", e.timed.amount);
                    j->second -= e.timed.amount;
                    state::l->newline(buff);
                }
                else{
                    state::l->newline("It is killed.\n");
                    effects.insert({DEAD, {0,0}});
                }
            }
            else{
                state::l->newline("It is can not be killed or broken.\n");
                return;
            }
            break;
        }

        case MAGIC_ATTACK:{
            if(effects.find(DEAD) != effects.end()){
                state::l->newline("Attakc magic will not make he alive.\n");
                return;
            }

            if(effects.find(MAGIC_IMMUNITY) != effects.end()){
                state::l->newline("Magic attack was not effect.\n");
                return;
            }

            auto i = effects.find(MAGIC_PROTECT);
            auto j = propertyes.find(HEALTH);

            if(i != effects.end()){
                state::l->newline("Antimagic aura decrease power of the magic.\n");
                if(i->second.timed.amount >= e.timed.amount){
                    state::l->newline("The magic was destroyed.\n");
                    return;
                }
                e.timed.amount -= i->second.timed.amount;
            }

            if((i = effects.find(MAGIC_WEAKNESS)) != effects.end()){
                state::l->newline("Object have weakness before magic.\n");
                e.timed.amount += i->second.timed.amount;
            }

            if(j != propertyes.end()){
                char buff[1024];
                const char* format;
                if(j->second <= e.timed.amount){
                    format = "This attack was letal.\n";
                    j->second = 0;
                    effects.insert({DEAD, {0,0}});
                }
                else{
                    format = "Received %d of magic damage.\n";
                    j->second -= e.timed.amount;
                }
                sprintf(buff, format, e.timed.amount);
                state::l->newline(buff);
            }
            else{
                state::l->newline("It can not be killed or broken.\n");
                return;
            }
            break;
        }

        case ELECTRIC_DAMAGE:{
            if(effects.find(DEAD) != effects.end()){
                state::l->newline("Defibrilation had not effect.\n");
                return;
            }

            if(effects.find(DIELECTRIC) != effects.end()){
                state::l->newline("It is dielectric.\n");
                return;
            }

            auto i = effects.find(ELECTRIC_PROTECT);
            auto j = propertyes.find(HEALTH);

            if(i != effects.end()){
                state::l->newline("Smh dielectric decreased damage.\n");
                if(i->second.timed.amount >= e.timed.amount){
                    state::l->newline("Dielectric neutralise all damage.\n");
                    return;
                }
                e.timed.amount -= i->second.timed.amount;
            }

            if((i = effects.find(ELECTRIC_WEAKNESS)) != effects.end()){
                state::l->newline("Object have weakness before electric.\n");
                e.timed.amount += i->second.timed.amount;
            }

            if((i = effects.find(WET)) != effects.end()){
                state::l->newline("Wet objects have weakeness before electric.\n");
                e.timed.amount *= 1.5;
            }

            if(j != propertyes.end()){
                char buff[1024];
                const char* format;
                if(j->second <= e.timed.amount){
                    format = "This attack was letal.\n";
                    j->second = 0;
                    effects.insert({DEAD, {0,0}});
                }
                else{
                    format = "Received %d of electric damage.\n";
                    j->second -= e.timed.amount;
                }
                sprintf(buff, format, e.timed.amount);
                state::l->newline(buff);
            }
            else{
                state::l->newline("It can not be killed or broken.\n");
                return;
            }
            break;
        }

        case FIRE_DAMAGE:{
            if(effects.find(DEAD) != effects.end()){
                state::l->newline("The fire will clean this object. Or not.\n");
                return;
            }

            if(effects.find(UNFLAMED) != effects.end()){
                state::l->newline("It can not burns.\n");
                return;
            }

            auto i = effects.find(FIRE_PROTECT);
            auto j = propertyes.find(HEALTH);

            if(i != effects.end()){
                state::l->newline("Fire goes weaker.\n");
                if(i->second.timed.amount >= e.timed.amount){
                    state::l->newline("Fire disappired.\n");
                    return;
                }
                e.timed.amount -= i->second.timed.amount;
            }

            if((i = effects.find(WET)) != effects.end()){
                state::l->newline("Wet objects whorse fireing.\n");
                e.timed.amount /= 2;
            }
            
            if((i = effects.find(FIRE_WEAKNESS)) != effects.end()){
                state::l->newline("Object is burning very good.\n");
                e.timed.amount += i->second.timed.amount;
            }


            if(j != propertyes.end()){
                char buff[1024];
                const char* format;
                if(j->second <= e.timed.amount){
                    format = "It was burned.\n";
                    j->second = 0;
                    effects.insert({DEAD, {0,0}});
                }
                else{
                    format = "Received %d of fire damage.\n";
                    j->second -= e.timed.amount;
                }
                sprintf(buff, format, e.timed.amount);
                state::l->newline(buff);
            }
            else{
                state::l->newline("It can not be burned.\n");
                return;
            }
            break;
        }

        default:
            state::l->newline("!! Logic is not realised yet !!\n");
            return;
    }
}