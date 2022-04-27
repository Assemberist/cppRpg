#include "card.hpp"

enum object_state{
    STAY,
    CHOOSE_SPELL,
    CHOOSE_TARGET
};

spell_t string_to_spell(const char* src){
    if(!strcmp(src, "Fire ball")) return FIREBALL;
    if(!strcmp(src, "Punch")) return PUNCH;
    if(!strcmp(src, "Lighting")) return LIGHTING;
}

blinking_cfg* search_targets(blinking_cfg* obj, screen s, size_t range){
    static blinking_cfg** last_target = NULL;

    if(!last_target) last_target = s.mapa->objects;

    if(obj){
        while(*last_target){
            if( abs(*last_target->X, obj->X) <= range &&
                abs(*last_target->X, obj->X) <= range )
                    return *last_target;

            last_target++;
        }
    }
    return last_target = NULL;
}

void non_user_turn(object* o, screen s){

}

void user_turn(object* u, screen s){
    object_state stat = STAY;
    spell_t choosed_spell;
    object* target;

    char temp = 0;
    while((temp = getch()) != ' '){
        s.mapa.clear();

        switch(stat){
            // moving and allowed choose of spell
            case STAY:
                switch(temp){
                    case 'q':
                    case 'w':
                    case 'e':
                    case 'a':
                    case 's':
                    case 'd':
                    case 'z':
                    case 'x':
                    case 'c':
                        s.mapa.move(u, temp);
                        return;
                    
                    case 'f':
                        u->print_spells(s.common_menu);
                        stat = CHOOSE_SPELL;
                        continue;

                    case -1:
                        s.mapa.update_card();
                        continue;
                }

            case CHOOSE_SPELL:{
                switch(temp){
                    case -1:
                        s.mapa.update_card();
                        break;

                    case 'f':{
                        choosed_spell = string_to_spell(s.common_menu.get_selected());
                        switch(choosed_spell){
                            case FIREBALL:
                                target = search_targets(s, 5);
                                break;

                            case PUNCH:
                                
                                break;

                            case LIGHTING:
                                
                                break;
                        }
                        break;
                    }
                }
            }
        }

        s.common_log.print();
        s.common_menu.print();
    }
}