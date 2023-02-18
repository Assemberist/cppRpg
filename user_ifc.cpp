#include <climits>
#include "user_ifc.hpp"
#include "spell.hpp"
#include "text_field.hpp"
#include "object_defs.hpp"

enum object_state{
    STAY,
    CHOOSE_SPELL,
    CHOOSE_TARGET,
    CHOOSE_TARGET_FOR_ITEM,
    OBSERVATION,
    LOOT,
    LOOKUP,
    SPELL_SHOP,
    SPELL_HELP,
    OPEN_INVENTORY,
    LOOT_INVENTORY
};

log* create_log_effects(object* obj){
    char buffer[70];
    size_t amount = obj->stat.effects.size();

    log* new_log = new log(10, 70, 1, 12);

    for(auto i = obj->stat.effects.begin(); i != obj->stat.effects.end(); i++){
        sprintf(buffer, "%30s Tim %4d Val %4d\n\0",
            effect_names[i->first.type],
            i->second.timed.time,
            i->second.timed.amount
        );

        new_log->newline(buffer);
    }

    return new_log;
}

log* create_log_properties(object* obj){
    char buffer[50];
    size_t amount = obj->stat.effects.size();

    log* new_log = new log(10, 50, 12, 12);

    for(auto i = obj->stat.propertyes.begin(); i != obj->stat.propertyes.end(); i++){
        sprintf(buffer, "%30s Val %4d\n\0",
            property_names[i->first],
            i->second
        );

        new_log->newline(buffer);
    }

    return new_log;
}

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

bool do_equp_unquip(inventory* inv, object* obj){
    if(inv->is_current_equiped()){
        if(obj->unequip(obj->equipment.begin() + inv->get_selected_value())){
            size_t i;
            bag_element* bag = create_bag(obj, i);
            inv->shrade_elements();
            inv->set_content(bag, i, item_names);
            return true;
        }
    }
    else{
        if(obj->equip(obj->inventory.begin() + inv->get_selected_value())){
            size_t i;
            bag_element* bag = create_bag(obj, i);
            inv->shrade_elements();
            inv->set_content(bag, i, item_names);
            return true;
        }
    }
    return false;
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

    inventory* active_inv;
    log* property_screen;
    log* effect_screen;

    object* tar = nullptr;
    object* looted_obj = nullptr;
    object* single_target = nullptr;
    blink_t last_color;
    

    char temp;
    while((temp = getch()) != ' '){
        switch(stat){
            // moving and choosing of spell are allowed
            case STAY:{
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
                        s.bag->activate(0);
                        s.bag->print();
                        u->graph_state = GREEN_STABILE;
                        stat = OPEN_INVENTORY;
                    }
                    break;

                    case 'o':
                        u->graph_state = GREEN_STABILE;
                        search_targets(NULL, s, NULL);
                        single_target = search_targets(u, s, INT_MAX);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;
                        stat = OBSERVATION;
                        break;

                    default:
                        if(is_move_char(temp)){
                            single_target = s.mapa->get_object(u, temp);
                            if(single_target){
                                size_t count;
                                bag_element* loot_bag = create_bag(single_target, count);
                                s.loot->set_content(loot_bag, count, item_names);
                                s.loot->activate(0);
                                s.loot->print();
                                u->graph_state = GREEN_STABILE;
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
            }

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
                        break;

                    case 'h':
                        // spell help
                        // put hint about choosed spell
                        stat = SPELL_HELP;
                }
            }
            break;

            case CHOOSE_TARGET_FOR_ITEM:{
                if(is_item_for_single_target(item_to_use->info.type_name)){
                    switch(temp){
                        case 's':
                            single_target->graph_state = last_color;
                            while(!(single_target = search_targets(u, s, 4)));
                            last_color = single_target->graph_state;
                            single_target->graph_state = RED_INVERT;
                            break;

                        case 'q':
                            single_target->graph_state = last_color;
                            single_target = looted_obj;
                            stat = last_state;

                            switch(last_state){
                                case LOOT:
                                    s.loot->print();
                                    break;
                                
                                case LOOT_INVENTORY:
                                    s.loot->print();

                                case OPEN_INVENTORY:
                                    s.bag->print();
                            }

                            break;

                        case 'f':{
                            single_target->graph_state = last_color;
                            for(spell_slot* s = item_to_use->slots; s; s = s->next)
                                if(s->config.trigger == BY_OWNER)
                                    ; // execute spells
                            
                            for(auto i = item_to_use->stat.effects.begin(); i != item_to_use->stat.effects.end(); i++){
                                if(get_effect_behavior(i->first) == SHARED){
                                    effect_def e = i->first;
                                    e.is_shared = false;
                                    u->act(e, i->second);
                                }
                            }
                            goto done;
                        }
                    }
                }
                else{
                    // marking of mutiple targets
                }

                s.mapa->clear();
                s.mapa->update_card();
                break;
            }

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

                    case THROW:
                        ranger = 4;
                        action = act_throw;
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
                                if(choosed_spell == THROW){
                                    single_target->pick_up_item(*item_to_use);
                                    looted_obj->inventory.erase(item_to_use);
                                }
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
                    break;
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
                        do_equp_unquip(s.bag, u);
                        s.bag->print();
                        break;
                    }

                    case 'u':{
                        if(s.bag->is_current_equiped()){
                            if(do_equp_unquip(s.bag, u)){
                                item_to_use = u->inventory.end() - 1;
                            }
                            else break;
                        }
                        else item_to_use = u->inventory.begin() + s.bag->get_selected_value();

                        looted_obj = u;

                        search_targets(NULL, s, 0);
                        single_target = search_targets(u, s, 4);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;

                        s.bag->hide();
                        s.mapa->clear();
                        s.mapa->update_card();

                        choosed_spell = NOTHING_SPELL;
                        last_state = OPEN_INVENTORY;
                        stat = CHOOSE_TARGET_FOR_ITEM;
                        break;
                    }

                    case 't':{
                        if(s.bag->is_current_equiped()){
                            if(do_equp_unquip(s.bag, u)){
                                item_to_use = u->inventory.end() - 1;
                            }
                            else break;
                        }
                        else item_to_use = u->inventory.begin() + s.bag->get_selected_value();

                        looted_obj = u;
                        u->item_to_use = &*item_to_use;

                        search_targets(NULL, s, 4);
                        single_target = search_targets(u, s, 4);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;

                        s.bag->hide();
                        s.mapa->clear();
                        s.mapa->update_card();

                        choosed_spell = THROW;
                        last_state = OPEN_INVENTORY;
                        stat = CHOOSE_TARGET;
                        break;
                    }

                    case 'q':
                        s.bag->hide();
                        s.bag->shrade_elements();
                        u->graph_state = GREEN_ON;
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
                        s.loot->hide();
                        s.loot->shrade_elements();
                        u->graph_state = GREEN_ON;
                        stat = STAY;
                        break;

                    case 's':
                        s.loot->up();
                        s.loot->print();
                        break;

                    case 'w':
                        s.loot->down();
                        s.loot->print();
                        break;

                    case 'e':{
                        do_equp_unquip(s.loot, single_target);
                        s.loot->print();
                        break;
                    }

                    case 'f':{
                        size_t num = s.loot->get_selected_value();
                        if(s.loot->is_current_equiped()){
                            if(do_equp_unquip(s.loot, single_target)){
                                u->pick_up_item(single_target->inventory.back());
                                single_target->inventory.pop_back();
                            }
                            else break;
                        }
                        else{
                            if(s.loot->size()){
                                u->pick_up_item(*(single_target->inventory.begin() + num));
                                single_target->inventory.erase(single_target->inventory.begin() + num);

                                s.loot->shrade_elements();
                                size_t count;
                                bag_element* tempor = create_bag(single_target, count);
                                s.loot->set_content(tempor, count, item_names);
                                s.loot->print();
                            }
                        }
                        break;
                    }

                    case 'u':{
                        if(s.loot->is_current_equiped()){
                            if(do_equp_unquip(s.loot, single_target)){
                                item_to_use = single_target->inventory.end() - 1;
                            }
                            else break;
                        }
                        else item_to_use = single_target->inventory.begin() + s.loot->get_selected_value();

                        looted_obj = single_target;

                        search_targets(NULL, s, 0);
                        single_target = search_targets(u, s, 4);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;

                        s.loot->hide();
                        s.mapa->clear();
                        s.mapa->update_card();

                        choosed_spell = NOTHING_SPELL;
                        last_state = LOOT;
                        stat = CHOOSE_TARGET_FOR_ITEM;
                        break;
                    }

                    case 't':{
                        if(s.loot->is_current_equiped()){
                            if(do_equp_unquip(s.loot, single_target)){
                                item_to_use = single_target->inventory.end() - 1;
                            }
                            else break;
                        }
                        else item_to_use = single_target->inventory.begin() + s.loot->get_selected_value();

                        looted_obj = single_target;
                        u->item_to_use = &*item_to_use;

                        search_targets(NULL, s, 0);
                        single_target = search_targets(u, s, 4);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;

                        s.loot->hide();
                        s.mapa->clear();
                        s.mapa->update_card();

                        choosed_spell = THROW;
                        last_state = LOOT;
                        stat = CHOOSE_TARGET;
                        break;
                    }

                    case 'i':{
                        size_t count;
                        bag_element* inv = create_bag(u, count);
                        s.bag->set_content(inv, count, item_names);
                        s.bag->deactivate();
                        s.bag->print();
                        active_inv = s.loot;
                        stat = LOOT_INVENTORY;
                        break;
                    }

                    case 'z':{
                        u->inventory.insert(u->inventory.cend(), single_target->inventory.begin(), single_target->inventory.end());
                        single_target->inventory.clear();

                        s.loot->shrade_elements();
                        s.loot->set_content(NULL, 0, item_names);
                        s.loot->print();
                        break;
                    }
                }
            }
            break;

            case LOOT_INVENTORY:{
                switch(temp){
                    case 'i':
                        s.bag->hide();
                        s.bag->shrade_elements();
                        stat = LOOT;
                        break;

                    case 'w':
                        active_inv->down();
                        active_inv->print();
                        break;

                    case 's':
                        active_inv->up();
                        active_inv->print();
                        break;

                    case 'a':
                        active_inv->deactivate();
                        s.bag->activate(active_inv->get_index());
                        active_inv = s.bag;
                        s.bag->print();
                        s.loot->print();
                        break;

                    case 'd':
                        active_inv->deactivate();
                        s.loot->activate(active_inv->get_index());
                        active_inv = s.loot;
                        s.bag->print();
                        s.loot->print();
                        break;

                    case 'u':
                        looted_obj = active_inv == s.bag ? u : single_target;

                        if(active_inv->is_current_equiped()){
                            if(do_equp_unquip(active_inv, looted_obj)){
                                item_to_use = looted_obj->inventory.end() - 1;
                            }
                            else break;
                        }
                        else item_to_use = looted_obj->inventory.begin() + active_inv->get_selected_value();

                        search_targets(NULL, s, 0);
                        single_target = search_targets(u, s, 4);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;

                        s.bag->hide();
                        s.loot->hide();
                        s.mapa->clear();
                        s.mapa->update_card();

                        choosed_spell = NOTHING_SPELL;
                        last_state = LOOT_INVENTORY;
                        stat = CHOOSE_TARGET_FOR_ITEM;
                        break;

                    case 't':
                        looted_obj = active_inv == s.bag ? u : single_target;

                        if(active_inv->is_current_equiped()){
                            if(do_equp_unquip(active_inv, looted_obj)){
                                item_to_use = looted_obj->inventory.end() - 1;
                            }
                            else break;
                        }
                        else item_to_use = looted_obj->inventory.begin() + active_inv->get_selected_value();

                        u->item_to_use = &*item_to_use;

                        search_targets(NULL, s, 0);
                        single_target = search_targets(u, s, 4);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;

                        s.bag->hide();
                        s.loot->hide();
                        s.mapa->clear();
                        s.mapa->update_card();

                        choosed_spell = THROW;
                        last_state = LOOT_INVENTORY;
                        stat = CHOOSE_TARGET;
                        break;

                    case 'e':{
                        if(active_inv->size() == 0) break;
                        object* tempor = active_inv == s.loot ? single_target : u;
                        do_equp_unquip(active_inv, tempor);
                        active_inv->print();
                        break;
                    }

                    case 'z':{
                        if(active_inv->size() == 0) break;

                        size_t i;
                        if(active_inv == s.loot){
                            u->inventory.insert(u->inventory.cend(), single_target->inventory.begin(), single_target->inventory.end());
                            single_target->inventory.clear();

                            bag_element* bag = create_bag(u, i);
                            s.bag->shrade_elements();
                            s.bag->set_content(bag, i, item_names);

                            s.loot->shrade_elements();
                            s.loot->set_content(NULL, 0, item_names);
                        }
                        else{
                            single_target->inventory.insert(single_target->inventory.cend(), u->inventory.begin(), u->inventory.end());
                            u->inventory.clear();

                            bag_element* bag = create_bag(single_target, i);
                            s.loot->shrade_elements();
                            s.loot->set_content(bag, i, item_names);

                            s.bag->shrade_elements();
                            s.bag->set_content(NULL, 0, item_names);
                        }

                        s.loot->print();
                        s.bag->print();
                        break;
                    }

                    case 'f':{
                        if(active_inv->size() == 0) break;
                                                
                        if(active_inv == s.loot){
                            if(active_inv->is_current_equiped()){
                                if(do_equp_unquip(active_inv, single_target)){
                                    u->pick_up_item(single_target->inventory.back());
                                    single_target->inventory.pop_back();
                                }
                                else break;
                            }
                            else{
                                vector<item>::iterator thing = single_target->inventory.begin() + active_inv->get_selected_value();
                                u->pick_up_item(*(thing));
                                single_target->inventory.erase(thing);
                            }
                        }
                        else{
                            if(active_inv->is_current_equiped()){
                                if(do_equp_unquip(active_inv, u)){
                                    single_target->pick_up_item(u->inventory.back());
                                    u->inventory.pop_back();
                                }
                                else break;
                            }
                            else{
                                vector<item>::iterator thing = u->inventory.begin() + active_inv->get_selected_value();
                                single_target->pick_up_item(*(thing));
                                u->inventory.erase(thing);
                            }
                        }

                        size_t i;
                        bag_element* bag = create_bag(u, i);
                        s.bag->shrade_elements();
                        s.bag->set_content(bag, i, item_names);
                        s.bag->print();

                        bag = create_bag(single_target, i);
                        s.loot->shrade_elements();
                        s.loot->set_content(bag, i, item_names);
                        s.loot->print();
                        break;
                    }

                    case 'q':
                        s.bag->hide();
                        s.bag->shrade_elements();
                        s.loot->hide();
                        s.loot->shrade_elements();
                        stat = STAY;
                        break;
                }
            }
            break;

            case LOOKUP:{
                switch(temp){
                    case 'q':
                        effect_screen->hide();
                        delete effect_screen;
                        property_screen->hide();
                        delete property_screen;
                        stat = OBSERVATION;
                        break;
                }
            }
            break;

            case OBSERVATION:{
                switch(temp){
                    case 's':
                        s.mapa->clear();
                        single_target->graph_state = last_color;
                        while(!(single_target = search_targets(u, s, INT_MAX)));
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;
                        s.mapa->update_card();
                        break;

                    case 'f':
                        effect_screen = create_log_effects(single_target);
                        effect_screen->print();
                        property_screen = create_log_properties(single_target);
                        property_screen->print();
                        stat = LOOKUP;
                        break;

                    case 'q':
                        u->graph_state = GREEN_ON;
                        stat = STAY;
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