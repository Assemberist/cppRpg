#include "state.hpp"

#ifdef DONT_LOG_STATE
    #define log_msg(A, B)
#else
    #define log_msg(A, B) if((A)){(A)->newline(B);}
#endif

void state::act(effect_def type, effect e){
    switch(type.type){
        case CRUSH_ATTACK:{
            if(effects.find({false, false, DEAD}) != effects.end()){
                log_msg(state::l, "Stop punch the dead body!\n");
                return;
            }

            if(effects.find({false, false, PHYSIC_IMMUNITY}) != effects.end()){
                log_msg(state::l, "Physic attacks had no effect.\n");
                return;
            }

            auto i = effects.find({false, false, PHYSIC_PROTECT});
            auto j = propertyes.find(HEALTH);
            
            if(i != effects.end()){
                log_msg(state::l, "Attacked object have physic resistance.\n");
                if(i->second.timed.amount > e.timed.amount){
                    log_msg(state::l, "The armory too strong to deal a damage.\n");
                    return;
                }
                e.timed.amount -= i->second.timed.amount;
            }
            
            if((i = effects.find({false, false, PHYSIC_WEAKNESS})) != effects.end()){
                log_msg(state::l, "Attacked object have physic weakness.\n");
                e.timed.amount += i->second.timed.amount;
            }

            if(j != propertyes.end()){
                if((i = effects.find({false, false, FRIZED})) != effects.end()){
                    if(j->second < e.timed.amount / 2){
                        log_msg(state::l, "It was splitted open.\n");
                        effects.insert({{false, false, DEAD}, {0, 0, 0}});
                        return;
                    }
                }
                
                if(j->second > e.timed.amount){
                #ifndef DONT_LOG_STATE
                    char buff[1024];
                    sprintf(buff, "Received %4d of physic damage.\n", e.timed.amount);
                    log_msg(state::l, buff);
                #endif
                    j->second -= e.timed.amount;
                }
                else{
                    log_msg(state::l, "It is killed.\n");
                    effects.insert({{false, false, DEAD}, {0, 0, 0}});
                }
            }
            else{
                log_msg(state::l, "It is can not be killed or broken.\n");
                return;
            }
            break;
        }

        case MAGIC_ATTACK:{
            if(effects.find({false, false, DEAD}) != effects.end()){
                log_msg(state::l, "Attakc magic will not make he alive.\n");
                return;
            }

            if(effects.find({false, false, MAGIC_IMMUNITY}) != effects.end()){
                log_msg(state::l, "Magic attack was not effect.\n");
                return;
            }

            auto i = effects.find({false, false, MAGIC_PROTECT});
            auto j = propertyes.find(HEALTH);

            if(i != effects.end()){
                log_msg(state::l, "Antimagic aura decrease power of the magic.\n");
                if(i->second.timed.amount >= e.timed.amount){
                    log_msg(state::l, "The magic was destroyed.\n");
                    return;
                }
                e.timed.amount -= i->second.timed.amount;
            }

            if((i = effects.find({false, false, MAGIC_WEAKNESS})) != effects.end()){
                log_msg(state::l, "Object have weakness before magic.\n");
                e.timed.amount += i->second.timed.amount;
            }

            if(j != propertyes.end()){
            #ifndef DONT_LOG_STATE
                char buff[1024];
                sprintf(buff, "Received %d of magic damage.\n", e.timed.amount);
                log_msg(state::l, buff);
            #endif
                if(j->second <= e.timed.amount){
                    log_msg(state::l, "This attack was letal.\n");
                    j->second = 0;
                    effects.insert({{false, false, DEAD}, {0, 0, 0}});
                }
                else{
                    j->second -= e.timed.amount;
                }
            }
            else{
                log_msg(state::l, "It can not be killed or broken.\n");
                return;
            }
            break;
        }

        case ELECTRIC_DAMAGE:{
            if(effects.find({false, false, DEAD}) != effects.end()){
                log_msg(state::l, "Defibrilation had not effect.\n");
                return;
            }

            if(effects.find({false, false, DIELECTRIC}) != effects.end()){
                log_msg(state::l, "It is dielectric.\n");
                return;
            }

            auto i = effects.find({false, false, ELECTRIC_PROTECT});
            auto j = propertyes.find(HEALTH);

            if(i != effects.end()){
                log_msg(state::l, "Smh dielectric decreased damage.\n");
                if(i->second.timed.amount >= e.timed.amount){
                    log_msg(state::l, "Dielectric neutralise all damage.\n");
                    return;
                }
                e.timed.amount -= i->second.timed.amount;
            }

            if((i = effects.find({false, false, ELECTRIC_WEAKNESS})) != effects.end()){
                log_msg(state::l, "Object have weakness before electric.\n");
                e.timed.amount += i->second.timed.amount;
            }

            if((i = effects.find({false, false, WET})) != effects.end()){
                log_msg(state::l, "Wet objects have weakeness before electric.\n");
                e.timed.amount *= 1.5;
            }

            if(j != propertyes.end()){
            #ifndef DONT_LOG_STATE
                char buff[1024];
                sprintf(buff, "Received %d of electric damage.\n", e.timed.amount);
                log_msg(state::l, buff);
            #endif
                if(j->second <= e.timed.amount){
                    log_msg(state::l, "This attack was letal.\n");
                    j->second = 0;
                    effects.insert({{false, false, DEAD}, {0, 0, 0}});
                }
                else{
                    j->second -= e.timed.amount;
                }
            }
            else{
                log_msg(state::l, "It can not be killed or broken.\n");
                return;
            }
            break;
        }

        case FIRE_DAMAGE:{
            if(effects.find({false, false, DEAD}) != effects.end()){
                log_msg(state::l, "The fire will clean this object. Or not.\n");
                return;
            }

            if(effects.find({false, false, UNFLAMED}) != effects.end()){
                log_msg(state::l, "It can not burns.\n");
                return;
            }

            auto i = effects.find({false, false, FIRE_PROTECT});
            auto j = propertyes.find(HEALTH);

            if(i != effects.end()){
                log_msg(state::l, "Fire goes weaker.\n");
                if(i->second.timed.amount >= e.timed.amount){
                    log_msg(state::l, "Fire disappired.\n");
                    return;
                }
                e.timed.amount -= i->second.timed.amount;
            }

            if((i = effects.find({false, false, WET})) != effects.end()){
                log_msg(state::l, "Wet objects whorse fireing.\n");
                e.timed.amount /= 2;
            }
            
            if((i = effects.find({false, false, FIRE_WEAKNESS})) != effects.end()){
                log_msg(state::l, "Object is burning very good.\n");
                e.timed.amount += i->second.timed.amount;
            }


            if(j != propertyes.end()){
            #ifndef DONT_LOG_STATE
                char buff[1024];
                sprintf(buff, "Received %d of fire damage.\n", e.timed.amount);
                log_msg(state::l, buff);
            #endif
                if(j->second <= e.timed.amount){
                    log_msg(state::l, "It was burned.\n");
                    j->second = 0;
                    effects.insert({{false, false, DEAD}, {0, 0, 0}});
                }
                else{
                    j->second -= e.timed.amount;
                }
            }
            else{
                log_msg(state::l, "It can not be burned.\n");
                return;
            }
            break;
        }

        default:
            log_msg(state::l, "!! Logic is not realised yet !!\n");
            return;
    }
}