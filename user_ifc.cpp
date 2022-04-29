#include "user_ifc.hpp"

void clear_blinking(blink_cfg* objs){
    for(int i=0; objs[i].o; i++){
        switch (objs[i].cfg.is_hide) {
            case GREEN_ON:
            case GREEN_OFF:
            case GREEN_INVERT:
            case GREEN_NO_INVERT:
                objs[i].cfg.is_hide = GREEN_STABILE;
                break;

            case RED_ON:
            case RED_OFF:
            case RED_INVERT:
            case RED_NO_INVERT:
                objs[i].cfg.is_hide = RED_STABILE;
                break;

            default:
                break;
        }
    }
}

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

bool user_turn(blink_cfg* u, screen s){
    u->cfg.is_hide = GREEN_ON;

    object_state stat = STAY;
    spell_t choosed_spell;
    target* tar = nullptr;
    blink_cfg* single_target = nullptr;

    char temp = 0;
    while((temp = getch()) != ' '){

        switch(stat){
            // moving and choosing of spell are allowed
            case STAY:
                s.mapa->clear();
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
                        goto done;
                    
                    case 'f':
                        u->o->print_spells(s.common_menu);
                        stat = CHOOSE_SPELL;
                        break;

                    default:
                        break;
                }
                s.mapa->update_card();
                break;

            case CHOOSE_SPELL:{
                switch(temp){
                    case -1:
                        s.mapa->clear();
                        s.mapa->update_card();
                        break;

                    case 'f':
                        choosed_spell = string_to_spell(s.common_menu->get_selected());
                        clear_blinking(s.mapa->objects);
                        switch (choosed_spell) {
                            case FIREBALL:
                                tar = new target(u->o->X, u->o->Y);
                                s.mapa->clear();
                                s.mapa->free_move(tar, temp);
                                s.mapa->draw_range(tar, 3);
                                break;

                            case PUNCH:
                                single_target = search_targets(u, s, 1);
                                break;

                            case LIGHTING:
                                single_target = search_targets(u, s, 5);
                                break;
                        }
                        stat = CHOOSE_TARGET;
                        break;

                    case 'q':
                        stat = STAY;
                        break;

                    case 'w':
                        s.common_menu->down();
                        s.common_menu->print();

                    case 's':
                        s.common_menu->up();
                        s.common_menu->print();
                }
            }

            case CHOOSE_TARGET:
                switch(choosed_spell){
                    case FIREBALL:{
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
                                s.mapa->clear();
                                s.mapa->free_move(tar, temp);
                                s.mapa->draw_range(tar, 3);
                                break;

                            case 'f':
                                for(int i=0; s.mapa->objects[i].o; i++)
                                    if( s.mapa->objects[i].cfg.is_hide == RED_INVERT ||
                                        s.mapa->objects[i].cfg.is_hide == GREEN_INVERT ){
                                            s.mapa->objects[i].o->act(FIRE_DAMAGE, {0, 25});
                                            s.mapa->objects[i].o->act(MAGIC_ATTACK, {0, 5}); }
                                break;

                            default:
                                continue;
                        }
                        break;
                    }

/*                    case PUNCH:
                        switch(temp){
                            case 's':
                                s.mapa->clear();
                                if(single_target->cfg.is_hide == GREEN_INVERT)
                                    single_target->cfg.is_hide = GREEN_STABILE;
                                else single_target->cfg.is_hide = RED_STABILE;

                                while(!(single_target = search_targets(u, s, 1)));

                                s.mapa->update_card();
                                break;

                            case 'f':
                                single_target->o->act(CRUSH_ATTACK, {0, 25});
                                
                                break;

                            case 'q':
                                stat = CHOOSE_SPELL;

                            default:
                                continue;
                        break;
                    }

                    case LIGHTING:
                        // copy from punch
                        break;
                        */
                }
                break;
        }
    }

done:

    if(tar) delete tar;

    u->cfg.is_hide = GREEN_STABILE;
    search_targets(nullptr, s, 0);

    return (temp == ' ' ? false : true);
}
