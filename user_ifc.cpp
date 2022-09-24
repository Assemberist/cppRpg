#include "user_ifc.hpp"
#include "spell.hpp"
#include "text_field.hpp"

enum object_state{
    STAY,
    CHOOSE_SPELL,
    CHOOSE_TARGET,
    OBSERVATION,
    LOOT,
    LOOKUP,
    SPELL_SHOP,
    SPELL_HELP,
    OPEN_INVENTORY,
    LOOT_INVENTORY
};

bag_element* create_bag(object* u, size_t& sas){
    sas = 0;
    bag_element* bagaje = new bag_element[u->inventory.size() + u->equipment.size() + 1];
    for(size_t i = u->equipment.size(); i--;){
        if(u->equipment[i].info.type_name == NOTHING_ITEM) continue;
        bagaje[sas].is_equiped = true;
        bagaje[sas].type = u->equipment[i].info.type_name;
        bagaje[sas].element = i;
        sas++;
    }
    for(size_t i = u->inventory.size(); i--;){
        bagaje[sas].is_equiped = false;
        bagaje[sas].type = u->inventory[i].info.type_name;
        bagaje[sas].element = i;
        sas++;
    }
    return bagaje;
}

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
    object_state last_state;
    spell_t choosed_spell = NOTHING_SPELL;
    vector<item>::iterator item_to_use;

    object* tar = nullptr;
    object* single_target = nullptr;
    blink_t last_color;
    

    char temp;
    while((temp = getch()) != ' '){
        switch(stat){
            // moving and choosing of spell are allowed
            case STAY:
                s.mapa->clear();
                switch(temp){
                    case 'f':{
                        auto sp = u->get_spells();
                        auto i = sp.begin();

                        menu_element* menuha = new menu_element[sp.size()];

                        // fucking copy-constructor!!!
                        for(size_t count = 0; i != sp.end(); i++)
                            menuha[count++] = *(menu_element*)&(*i);

                        s.common_menu->set_content(menuha, sp.size(), spell_names);
                        s.common_menu->print();
                        stat = CHOOSE_SPELL;
                    }
                    break;

                    case 'i':{
                        s.common_log->hide();
                        size_t count;
                        bag_element* bagaje = create_bag(u, count);
                        s.bag->set_content(bagaje, count, item_names);
                        s.bag->print();
                        stat = OPEN_INVENTORY;
                    }
                    break;

                    case 'o':
                        break;

                    default:
                        if(is_move_char(temp)){
                            single_target = s.mapa->get_object(u, temp);
                            if(single_target){
                                size_t count;
                                bag_element* loot_bag = create_bag(single_target, count);
                                s.loot->set_content(loot_bag, count, item_names);
                                s.loot->print();
                                stat = LOOT;
                            }
                            else{
                                s.mapa->move(u, temp);
                                goto done;
                            }
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
                        choosed_spell = (spell_t)s.common_menu->get_selected_key();
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
                        choosed_spell = NOTHING_SPELL;
                        stat = STAY;
                        s.common_menu->shrade_elements();
                        s.common_menu->hide();
                        break;

                    case 'w':
                        s.common_menu->down();
                        s.common_menu->print();
                        break;

                    case 's':
                        s.common_menu->up();
                        s.common_menu->print();

                    case 'h':
                        // spell help
                        // put hint about choosed spell
                        stat = SPELL_HELP;
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
                                    single_target->stat.act({0,1,FIRE_DAMAGE}, {0, 25});
                                    single_target->stat.act({0,1,MAGIC_ATTACK}, {0, 5});
                                }

                            #if !defined(DONT_LOG_ACTIONS) || !defined(DONT_LOG_STATE)
                                s.common_log->print();
                            #endif

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

                            #if !defined(DONT_LOG_ACTIONS) || !defined(DONT_LOG_STATE)
                                s.common_log->print();
                            #endif

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

                    case NOTHING_SPELL:{
                        if(is_item_for_single_target(item_to_use->info.type_name)){
                            switch(temp){
                                case 's':
                                    single_target = search_targets(u, s, 4);
                                    last_color = single_target->graph_state;
                                    single_target->graph_state = RED_INVERT;
                                    s.mapa->update_card();
                                    break;

                                case 'q':
                                    stat = OPEN_INVENTORY;
                                    single_target->graph_state = last_color;
                                    single_target = nullptr;
                                    break;

                                case 'f':{
                                    single_target->graph_state = last_color;
                                    for(auto i = item_to_use->stat.effects.begin(); i != item_to_use->stat.effects.end(); i++){
                                        switch(get_effect_behavior(i->first)){
                                            case SHARED:
                                                if(i->second.timed.time == 0){
                                                    item_to_use->stat.effects.erase(i->first);
                                                    break;
                                                }

                                            case SHARMANENT:
                                                if(i->second.timed.time){
                                                    effect_def def = i->first;
                                                    def.is_permanent = false;

                                                    effect eff = i->second;
                                                    eff.timed.time = 0;
                                                    single_target->act(def, eff);

                                                    i->second.timed.time--;
                                                }
                                            break;

                                            case PERMANENT:{
                                                effect_def def = i->first;
                                                def.is_permanent = false;
                                                single_target->act(def, i->second);
                                            }
                                            break;

                                            case PURE:
                                                single_target->act(i->first, i->second);
                                                item_to_use->stat.effects.erase(i->first);
                                            break;
                                        }
                                    }
                                    goto done;
                                }
                            }
                        }
                        else{
                            // marking of mutiple targets
                        }
                    }
                }
                break;
            }

            case OPEN_INVENTORY:{
                switch(temp){
                    case 's':
                        s.bag->up();
                        s.bag->print();
                        break;

                    case 'w':
                        s.bag->down();
                        s.bag->print();
                        break;

                    case 'e':{
                        if(s.bag->is_current_equiped()){
                            if(u->unequip(u->equipment.begin() + s.bag->get_selected_value()))
                                s.bag->invert_equip();
                        }
                        else{
                            if(u->equip(u->inventory.begin() + s.bag->get_selected_value()))
                                s.bag->invert_equip();
                        }
                        s.bag->print();
                        break;
                    }

                    case 'f':
                        break;

                    case 't':
                        break;

                    case 'q':
                        s.bag->hide();
                        s.bag->shrade_elements();
                        stat = STAY;
                        break;
                }
            }
            break;

            case SPELL_HELP:
                switch(temp){
                    case 'q':
                        // clean hint
                        // need to return to right state.
                        // stat = CHOOSE_SPELL : SPELL_SHOP;
                        10;
                }
                break;

            case LOOT:{
                switch(temp){
                    case 'q':
                        break;

                    case 'i':
                        break;
                }
            }
            break;

            case LOOT_INVENTORY:{
                switch(temp){
                    case 'i':
                        break;

                    case 'u':
                        break;

                    case 'q':
                        break;
                }
            }
            break;

            case LOOKUP:{
                switch(temp){
                    case 'q':
                        break;
                }
            }
            break;

            case OBSERVATION:{
                switch(temp){
                    case 'f':
                        break;

                    case 'q':
                        break;
                }
            }
            break;

            case SPELL_SHOP:{
                switch(temp){
                    case 'i':
                        break;
                }
            }
            break;
        }
    }

done:

    if(tar) delete tar;

    u->graph_state = original_color;
    s.mapa->clear();
    s.mapa->update_card();
    s.common_menu->hide();
    s.common_menu->shrade_elements();
    
    search_targets(nullptr, s, 0);

    return (temp == ' ' ? false : true);
}