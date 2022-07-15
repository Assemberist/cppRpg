#include "user_ifc.hpp"
#include "spell.hpp"

enum object_state{
    STAY,
    CHOOSE_SPELL,
    CHOOSE_TARGET
};

void clear_blinking(object** objs){
    for(int i=0; objs[i]; i++){
        switch (objs[i]->graph_state) {
            case GREEN_ON:
            case GREEN_OFF:
            case GREEN_INVERT:
            case GREEN_NO_INVERT:
                objs[i]->graph_state = GREEN_STABILE;
                break;

            case RED_ON:
            case RED_OFF:
            case RED_INVERT:
            case RED_NO_INVERT:
                objs[i]->graph_state = RED_STABILE;
                break;

            default:
                break;
        }
    }
}

bool is_move_char(char direction){
    switch(direction){
        case 'q':
        case 'w':
        case 'e':
        case 'a':
        case 's':
        case 'd':
        case 'z':
        case 'x':
        case 'c':
            return true;
        
        default:
            return false;
    }
}

bool user_turn(object* u, screen s){
    blink_t original_color = u->graph_state;

    u->graph_state = GREEN_ON;

    object_state stat = STAY;
    spell_t choosed_spell;
    object* tar = nullptr;
    object* single_target = nullptr;
    blink_t last_color;

    auto sp = u->get_spells();
    s.common_menu->set_content((menu_element**)(&sp[0]), sp.size());

    char temp;
    while((temp = getch()) != ' '){
        switch(stat){
            // moving and choosing of spell are allowed
            case STAY:
                s.mapa->clear();
                switch(temp){
                    case 'f':
                        s.common_menu->print();
                        stat = CHOOSE_SPELL;
                        break;

                    default:
                        if(is_move_char(temp)){
                            s.mapa->move(u, temp);
                            goto done;
                        }
                        else break;
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
                        choosed_spell = (spell_t)reinterpret_cast<long>(s.common_menu->get_selected());
                        clear_blinking(s.mapa->objects);
                        u->graph_state = GREEN_STABILE;
                        s.mapa->clear();
                        switch (choosed_spell) {
                            case FIREBALL:
                                tar = new target(u->X, u->Y);
                                tar->graph_state = HIDE;
                                s.mapa->free_move(tar, 's');
                                s.mapa->draw_range(tar, 3);
                                break;

                            case PUNCH:
                                single_target = search_targets(u, s, 1);
                                last_color = single_target->graph_state;
                                single_target->graph_state = RED_INVERT;
                                s.mapa->update_card();
                                break;

                            case LIGHTING:
                                single_target = search_targets(u, s, 5);
                                last_color = single_target->graph_state;
                                single_target->graph_state = RED_INVERT;
                                s.mapa->update_card();
                                break;
                        }
                        s.common_menu->hide();
                        stat = CHOOSE_TARGET;
                        break;

                    case 'q':
                        stat = STAY;
                        s.common_menu->hide();
                        break;

                    case 'w':
                        s.common_menu->down();
                        s.common_menu->print();
                        break;

                    case 's':
                        s.common_menu->up();
                        s.common_menu->print();
                }
            }
            break;

            case CHOOSE_TARGET:{
                int8_t ranger;
                void(*action)(object*, object*);

                switch(choosed_spell){
                    case FIREBALL:{
                        switch(temp){
                            case 'f':
                                search_targets(nullptr, s, 0);
                                while(single_target = search_targets(tar, s, 2)){
                                    single_target->act(FIRE_DAMAGE, {0, 25});
                                    single_target->act(MAGIC_ATTACK, {0, 5});
                                }

                                s.common_log->print();
                                goto done;

                            case 'r':
                                stat = CHOOSE_SPELL;
                                s.mapa->clear();
                                u->graph_state = GREEN_ON;
                                s.common_menu->print();
                                search_targets(nullptr, s, 0);
                                s.mapa->update_card();
                                break;

                            default:
                                if(is_move_char(temp)){
                                    s.mapa->clear();
                                    s.mapa->free_move(tar, temp);
                                    s.mapa->draw_range(tar, 3);
                                    break;
                                }
                                else continue;
                        }
                        break;
                    }
                    break;

                    case PUNCH:
                        ranger = 1;
                        action = act_punch;
                        goto act_with_target;

                    case LIGHTING:
                        ranger = 5;
                        action = act_lighting;

                    act_with_target:
                        switch(temp){
                            case 's':
                                s.mapa->clear();
                                single_target->graph_state = last_color;
                                while(!(single_target = search_targets(u, s, ranger)));
                                last_color = single_target->graph_state;
                                single_target->graph_state = RED_INVERT;
                                s.mapa->update_card();
                                break;

                            case 'f':
                                action(u, single_target);
                                single_target->graph_state = last_color;
                                s.common_log->print();
                                goto done;

                            case 'q':
                                stat = CHOOSE_SPELL;
                                s.mapa->clear();
                                u->graph_state = GREEN_ON;
                                s.common_menu->print();
                                s.mapa->update_card();
                                search_targets(nullptr, s, 0);
                                break;

                            default:
                                continue;
                        break;
                    }
                }
                break;
            }
        }
    }

done:

    if(tar) delete tar;

    u->graph_state = original_color;
    s.mapa->clear();
    s.mapa->update_card();
    s.common_menu->hide();
    search_targets(nullptr, s, 0);

    return (temp == ' ' ? false : true);
}