#include "object.hpp"

#define MAX_LINES 10

static const char* lines[10];

const char** act(object* obj, effect_t type, effect e){
    size_t num = 0;

    switch(type){
        case CRUSH_ATTACK:{
            if(obj->effects.find(DEAD) != obj->effects.end()){
                lines[num++] = "Stop punch the dead body!\n";
                lines[num] = NULL;
                return lines;
            }

            if(obj->effects.find(PHYSIC_IMMUNITY) != obj->effects.end()){
                lines[num++] = "Physic attacks had no effect.\n";
                lines[num] = NULL;
                return lines;
            }

            auto i = obj->effects.find(PHYSIC_PROTECT);
            auto j = obj->propertyes.find(HEALTH);
            
            if(i != obj->effects.end()){
                lines[num++] = "Attacked object have physic resistance.\n";
                if(i->second.timed.amount > e.timed.amount){
                    lines[num++] = "The armory too strong to deal a damage.\n";
                    lines[num] = NULL;
                    return lines;
                }
                e.timed.amount -= i->second.timed.amount;
            }
            
            if((i = obj->effects.find(PHYSIC_WEAKNESS)) != obj->effects.end()){
                lines[num++] = "Attacked object have physic weakness.\n";
                e.timed.amount += i->second.timed.amount;
            }

            if(j != obj->propertyes.end()){
                if((i = obj->effects.find(FRIZED)) != obj->effects.end()){
                    if(j->second < e.timed.amount / 2){
                        lines[num++] = "It was splitted open.\n";
                        obj->effects.insert({DEAD, {0, 0}});
                        lines[num] = NULL;
                        return lines;
                    }
                }
                
                if(j->second > e.timed.amount){
                    char buff[1024];
                    sprintf(buff, "%s got %4d of physic damage.\n", name.c_str(), e.timed.amount);
                    j->second -= e.timed.amount;
                    lines[num++] = buff;
                }
                else{
                    char buff[1024];
                    sprintf(buff, "%s was killed.\n", name.c_str());
                    lines[num++] = buff;
                    obj->effects.insert({DEAD, {0,0}});
                }
            }
            else{
                lines[num++] = "It is can not be killed or broken.\n";
                lines[num] = NULL;
                return lines;
            }
            break;
        }

        case MAGIC_ATTACK:{
            if(obj->effects.find(DEAD) != obj->effects.end()){
                lines[num++] = "Attakc magic will not make he alive.\n";
                lines[num] = NULL;
                return lines;
            }

            if(obj->effects.find(MAGIC_IMMUNITY) != obj->effects.end()){
                lines[num++] = "Magic attack was not effect.\n";
                lines[num] = NULL;
                return lines;
            }

            auto i = obj->effects.find(MAGIC_PROTECT);
            auto j = obj->propertyes.find(HEALTH);

            if(i != obj->effects.end()){
                lines[num++] = "Antimagic aura decrease power of the magic.\n";
                if(i->second.timed.amount >= e.timed.amount){
                    lines[num++] = "The magic was destroyed.\n";
                    lines[num] = NULL;
                    return lines;
                }
                e.timed.amount -= i->second.timed.amount;
            }

            if((i = obj->effects.find(MAGIC_WEAKNESS)) != obj->effects.end()){
                lines[num++] = "Object have weakness before magic.\n";
                e.timed.amount += i->second.timed.amount;
            }

            if(j != obj->propertyes.end()){
                char buff[1024];
                const char* format;
                if(j->second <= e.timed.amount){
                    format = "This attack was letal for the %s.\n";
                    j->second = 0;
                    obj->effects.insert({DEAD, {0,0}});
                }
                else{
                    format = "%s received %d of magic damage.\n";
                    j->second -= e.timed.amount;
                }
                sprintf(buff, format, name.c_str(), e.timed.amount);
                lines[num++] = buff;
            }
            else{
                lines[num++] = "It can not be killed or broken.\n";
                lines[num] = NULL;
                return lines;
            }
            break;
        }

        case ELECTRIC_DAMAGE:{
            if(obj->effects.find(DEAD) != obj->effects.end()){
                lines[num++] = "Defibrilation had not effect.\n";
                lines[num] = NULL;
                return lines;
            }

            if(obj->effects.find(DIELECTRIC) != obj->effects.end()){
                lines[num++] = "It is dielectric.\n";
                lines[num] = NULL;
                return lines;
            }

            auto i = obj->effects.find(ELECTRIC_PROTECT);
            auto j = obj->propertyes.find(HEALTH);

            if(i != obj->effects.end()){
                lines[num++] = "Smh dielectric decreased damage.\n";
                if(i->second.timed.amount >= e.timed.amount){
                    lines[num++] = "Dielectric neutralise all damage.\n";
                    lines[num] = NULL;
                    return lines;
                }
                e.timed.amount -= i->second.timed.amount;
            }

            if((i = obj->effects.find(ELECTRIC_WEAKNESS)) != obj->effects.end()){
                lines[num++] = "Object have weakness before electric.\n";
                e.timed.amount += i->second.timed.amount;
            }

            if((i = obj->effects.find(WET)) != obj->effects.end()){
                lines[num++] = "Wet objects have weakeness before electric.\n";
                e.timed.amount *= 1.5;
            }

            if(j != obj->propertyes.end()){
                char buff[1024];
                const char* format;
                if(j->second <= e.timed.amount){
                    format = "This attack was letal for the %s.\n";
                    j->second = 0;
                    obj->effects.insert({DEAD, {0,0}});
                }
                else{
                    format = "%s received %d of electric damage.\n";
                    j->second -= e.timed.amount;
                }
                sprintf(buff, format, name.c_str(), e.timed.amount);
                lines[num++] = buff;
            }
            else{
                lines[num++] = "It can not be killed or broken.\n";
                lines[num] = NULL;
                return lines;
            }
            break;
        }

        case FIRE_DAMAGE:{
            if(obj->effects.find(DEAD) != obj->effects.end()){
                lines[num++] = "The fire will clean this object. Or not.\n";
                lines[num] = NULL;
                return lines;
            }

            if(obj->effects.find(UNFLAMED) != obj->effects.end()){
                lines[num++] = "It can not burns.\n";
                lines[num] = NULL;
                return lines;
            }

            auto i = obj->effects.find(FIRE_PROTECT);
            auto j = obj->propertyes.find(HEALTH);

            if(i != obj->effects.end()){
                lines[num++] = "Fire goes weaker.\n";
                if(i->second.timed.amount >= e.timed.amount){
                    lines[num++] = "Fire disappired.\n";
                    lines[num] = NULL;
                    return lines;
                }
                e.timed.amount -= i->second.timed.amount;
            }

            if((i = obj->effects.find(WET)) != obj->effects.end()){
                lines[num++] = "Wet objects whorse fireing.\n";
                e.timed.amount /= 2;
            }
            
            if((i = obj->effects.find(FIRE_WEAKNESS)) != obj->effects.end()){
                lines[num++] = "Object is burning very good.\n";
                e.timed.amount += i->second.timed.amount;
            }


            if(j != obj->propertyes.end()){
                char buff[1024];
                const char* format;
                if(j->second <= e.timed.amount){
                    format = "The %s was burned.\n";
                    j->second = 0;
                    obj->effects.insert({DEAD, {0,0}});
                }
                else{
                    format = "%s received %d of fire damage.\n";
                    j->second -= e.timed.amount;
                }
                sprintf(buff, format, name.c_str(), e.timed.amount);
                lines[num++] = buff;
            }
            else{
                lines[num++] = "It can not be burned.\n";
                lines[num] = NULL;
                return lines;
            }
            break;
        }

        default:
            lines[num++] = "!! Logic is not realised yet !!\n";
            lines[num] = NULL;
            return lines;
    }
}