#include "card.hpp"

enum object_state{
    STAY,
    CHOOSE_SPELL,
    CHOOSE_TARGET
};

spell_t string_to_spell(const char* src){
    if(!strcmp(src, "Fire ball")) return FIREBALL;
    if(!strcmp(src, "Punch")) return PUNCH;
    //if(!strcmp(src, "Lighting")); 
    return LIGHTING;
}

blink_cfg* search_targets(blink_cfg* obj, screen s, size_t range){
    static blink_cfg* last_target = NULL;

    if(!last_target) last_target = s.mapa->objects;

    if(obj){
        while(last_target->o){
            if(abs(last_target->o->X, obj->o->X) <= range)
                if(abs(last_target->o->Y, obj->o->Y) <= range){
                    last_target++;
                    return last_target-1;
                }

            last_target++;
        }
    }
    return last_target = NULL;
}

void non_user_turn(object* o, screen s){

}

void user_turn(blink_cfg* u, screen s){
    u->cfg.is_hide = GREEN_ON;

    object_state stat = STAY;
    spell_t choosed_spell;
    blink_cfg* target;

    char temp = 0;
    while((temp = getch()) != ' '){
        s.mapa->clear();

        switch(stat){
            // moving and choosing of spell are allowed
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
                        s.mapa->move(u->o, temp);
                        return;
                    
                    case 'f':
                        u->o->print_spells(s.common_menu);
                        stat = CHOOSE_SPELL;
                        continue;

                    case -1:
                        s.mapa->update_card();
                        continue;
                }

            case CHOOSE_SPELL:{
                switch(temp){
                    case -1:
                        s.mapa->update_card();
                        break;

                    case 'f':{
                        choosed_spell = string_to_spell(s.common_menu->get_selected());
                        switch(choosed_spell){
                            case FIREBALL:{
                                target = u;
                                blink_cfg* tmp = search_targets(target, s, 3);
                                while(tmp)
                                    target->cfg.is_hide = RED_ON;
                                break;
                            }

                            case PUNCH:
                                
                                break;

                            case LIGHTING:
                                
                                break;
                        }
                        break;
                    }

                    case 'w':
                        s.common_menu->down();
                        s.common_menu->print();

                    case 's':
                        s.common_menu->up();
                        s.common_menu->print();
                }
            }

            case CHOOSE_TARGET:
                break;
        }

        s.common_log->print();
        s.common_menu->print();
    }
    u->cfg.is_hide = GREEN_STABILE;
}