#include "object.hpp"

void object::act(effect e){
    switch(e.type){
        case CRUSH_ATTACK:
            for(auto i = effects.begin(); i != effects.end(); i++)
                switch(i->type){
                    case PHYSIC_IMMUNITY:
                        return;
                }
            break;
    }
}

void object::calculate(){

}