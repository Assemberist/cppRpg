#include "state.hpp"

#ifdef DONT_LOG_STATE
    #define log_msg(A, B)
    #define log_construct(...)
#else
    char buff[1024];

    #define log_construct sprintf
    #define log_msg(A, B) if((A)){(A)->newline(B);}
#endif

void state::act(effect_def type, effect e){
    switch(type.type){
        case CRUSH_ATTACK:{
            if(effects_perm.find({false, DEAD}) != effects_perm.end()){
                log_msg(state::l, "Stop punch the dead body!\n");
                return;
            }

            if(effects.find({false, PHYSIC_IMMUNITY}) != effects.end()){
                log_msg(state::l, "Physic attacks had no effect.\n");
                return;
            }

            auto i = effects.find({false, PHYSIC_PROTECT});
            auto j = effects_perm.find({0, HEALTH});
            
            if(i != effects.end()){
                log_msg(state::l, "Attacked object have physic resistance.\n");
                if(i->second.amount > e.amount){
                    log_msg(state::l, "The armory too strong to deal a damage.\n");
                    return;
                }
                e.amount -= i->second.amount;
            }
            
            if((i = effects.find({false, PHYSIC_WEAKNESS})) != effects.end()){
                log_msg(state::l, "Attacked object have physic weakness.\n");
                e.amount += i->second.amount;
            }

            if(j != effects_perm.end()){
                if((i = effects.find({false, FRIZED})) != effects.end()){
                    if(j->second < e.amount / 2){
                        log_msg(state::l, "It was splitted open.\n");
                        effects_perm.insert({{false, DEAD}, 0});
                        return;
                    }
                }
                
                if(j->second > e.amount){
                    log_construct(buff, "Received %4d of physic damage.\n", e.amount);
                    log_msg(state::l, buff);
                    j->second -= e.amount;
                }
                else{
                    log_msg(state::l, "It is killed.\n");
                    effects_perm.insert({{false, DEAD}, 0});
                }
            }
            else{
                log_msg(state::l, "It is can not be killed or broken.\n");
                return;
            }
            break;
        }

        case MAGIC_ATTACK:{
            if(effects_perm.find({false, DEAD}) != effects_perm.end()){
                log_msg(state::l, "Attakc magic will not make he alive.\n");
                return;
            }

            if(effects.find({false, MAGIC_IMMUNITY}) != effects.end()){
                log_msg(state::l, "Magic attack was not effect.\n");
                return;
            }

            auto i = effects_perm.find({false, MAGIC_PROTECT});
            auto j = effects_perm.find({0, HEALTH});

            if(i != effects_perm.end()){
                log_msg(state::l, "Antimagic aura decrease power of the magic.\n");
                if(i->second >= e.amount){
                    log_msg(state::l, "The magic was destroyed.\n");
                    return;
                }
                e.amount -= i->second;
            }

            if((i = effects_perm.find({false, MAGIC_WEAKNESS})) != effects_perm.end()){
                log_msg(state::l, "Object have weakness before magic.\n");
                e.amount += i->second;
            }

            if(j != effects_perm.end()){
                log_construct(buff, "Received %d of magic damage.\n", e.amount);
                log_msg(state::l, buff);

                if(j->second <= e.amount){
                    log_msg(state::l, "This attack was letal.\n");
                    j->second = 0;
                    effects_perm.insert({{false, DEAD}, 0});
                }
                else{
                    j->second -= e.amount;
                }
            }
            else{
                log_msg(state::l, "It can not be killed or broken.\n");
                return;
            }
            break;
        }

        case ELECTRIC_DAMAGE:{
            if(effects_perm.find({false, DEAD}) != effects_perm.end()){
                log_msg(state::l, "Defibrilation had not effect.\n");
                return;
            }

            if(effects.find({false, DIELECTRIC}) != effects.end()){
                log_msg(state::l, "It is dielectric.\n");
                return;
            }

            auto i = effects.find({false, ELECTRIC_PROTECT});
            auto j = effects_perm.find({0, HEALTH});

            if(i != effects.end()){
                log_msg(state::l, "Smh dielectric decreased damage.\n");
                if(i->second.amount >= e.amount){
                    log_msg(state::l, "Dielectric neutralise all damage.\n");
                    return;
                }
                e.amount -= i->second.amount;
            }

            if((i = effects.find({false, ELECTRIC_WEAKNESS})) != effects.end()){
                log_msg(state::l, "Object have weakness before electric.\n");
                e.amount += i->second.amount;
            }

            if((i = effects.find({false, WET})) != effects.end()){
                log_msg(state::l, "Wet objects have weakeness before electric.\n");
                e.amount *= 1.5;
            }

            if(j != effects_perm.end()){
                log_construct(buff, "Received %d of electric damage.\n", e.amount);
                log_msg(state::l, buff);

                if(j->second <= e.amount){
                    log_msg(state::l, "This attack was letal.\n");
                    j->second = 0;
                    effects_perm.insert({{false, DEAD}, 0});
                }
                else{
                    j->second -= e.amount;
                }
            }
            else{
                log_msg(state::l, "It can not be killed or broken.\n");
                return;
            }
            break;
        }

        case FIRE_DAMAGE:{
            if(effects_perm.find({false, DEAD}) != effects_perm.end()){
                log_msg(state::l, "The fire will clean this object. Or not.\n");
                return;
            }

            if(effects.find({false, UNFLAMED}) != effects.end()){
                log_msg(state::l, "It can not burns.\n");
                return;
            }

            auto i = effects.find({false, FIRE_PROTECT});
            auto j = effects_perm.find({0, HEALTH});

            if(i != effects.end()){
                log_msg(state::l, "Fire goes weaker.\n");
                if(i->second.amount >= e.amount){
                    log_msg(state::l, "Fire disappired.\n");
                    return;
                }
                e.amount -= i->second.amount;
            }

            if((i = effects.find({false, WET})) != effects.end()){
                log_msg(state::l, "Wet objects whorse fireing.\n");
                e.amount /= 2;
            }
            
            if((i = effects.find({false, FIRE_WEAKNESS})) != effects.end()){
                log_msg(state::l, "Object is burning very good.\n");
                e.amount += i->second.amount;
            }

            if(j != effects_perm.end()){
                log_construct(buff, "Received %d of fire damage.\n", e.amount);
                log_msg(state::l, buff);

                if(j->second <= e.amount){
                    log_msg(state::l, "It was burned.\n");
                    j->second = 0;
                    effects_perm.insert({{false, DEAD}, 0});
                }
                else{
                    j->second -= e.amount;
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