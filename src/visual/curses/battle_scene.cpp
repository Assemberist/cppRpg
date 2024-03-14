#include <climits>
#include "battle_scene.hpp"

static spell_menu* common_menu;
static inventory* bag;
static inventory* loot;
static inventory_with_owner* observe_menu;
static text_log* manual;

void init_graphic(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
}

screen::screen(char* card, drawable_object** objs){
    common_log = new text_log(10, 50, 0, 11);
    object::set_log(common_log);
    state::set_log(common_log);

    mapa = new za_mapo(10, 10, 0, 0);

    common_menu = new spell_menu(3, 50, 12, 0);
    bag = new inventory(10, 50, 0, 11);
    loot = new inventory(10, 50, 0, 63);
    observe_menu = new inventory_with_owner(10, 50, 0, 11);
    manual = new text_log(12, 40, 12, 63);
    refresh();
    print_help_for_stay(manual);

    strcpy((char*)mapa->mapa, card);
    mapa->objects = (drawable_object**)objs;

    mapa->init_palitra();
    mapa->update_card();

    timeout(500);
}

screen::~screen(){
    timeout(-1);
    endwin();
}

enum object_state{
    STAY,
    CHOOSE_SPELL,
    CHOOSE_TARGET,
    CHOOSE_STATE_FOR_OBERVATION,
    CHOOSE_TARGET_FOR_ITEM,
    OBSERVATION,
    LOOT,
    LOOKUP,
    SPELL_SHOP,
    //SPELL_HELP,
    OPEN_INVENTORY,
    LOOT_INVENTORY
};

text_log* create_log_effects(state* stat){
    char buffer[70];

    text_log* new_log = new text_log(10, 70, 1, 12);

    for(auto i = stat->effects.begin(); i != stat->effects.end(); i++){
        sprintf(buffer, "%30s Tim %4d Val %4d\n",
            get_enum_name(i->first.type),
            i->second.time,
            i->second.amount
        );

        new_log->newline(buffer);
    }

    return new_log;
}

text_log* create_log_properties(state* stat){
    char buffer[50];

    text_log* new_log = new text_log(10, 50, 12, 12);

    for(auto i = stat->effects_perm.begin(); i != stat->effects_perm.end(); i++){
        sprintf(buffer, "%30s Val %4d\n",
            get_enum_name(i->first.type),
            i->second
        );

        new_log->newline(buffer);
    }

    return new_log;
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
        if(obj->unequip(inv->get_selected_value().it)){
            inv->build_content(obj);
            return true;
        }
    }
    else{
        if(obj->equip(inv->get_selected_value().it)){
            inv->build_content(obj);
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

object* search_targets(object* obj, screen s, size_t range){
    static object** last_target = NULL;

    if(!last_target) last_target = (object**)s.mapa->objects;

    if(obj){
        while(*last_target){
            if(abs((*last_target)->X, obj->X) <= range)
                if(abs((*last_target)->Y, obj->Y) <= range){
                    last_target++;
                    return *(last_target-1);
                }

            last_target++;
        }
    }

    last_target = NULL;
    return NULL;
}

bool user_turn(object* u, screen s){
    blink_t original_color = u->graph_state;

    u->graph_state = GREEN_ON;

    object_state stat = STAY;
    object_state last_state;
    spell_t choosed_spell = NOTHING_SPELL;
    vector<item>::iterator item_to_use;

    inventory* active_inv;
    text_log* oneline_log;
    text_log* property_screen;
    text_log* effect_screen;

    object* tar = nullptr;
    object* looted_obj = nullptr;
    object* single_target = nullptr;
    blink_t last_color;
    
    state* observed_state = nullptr;

    char temp;
    while((temp = getch()) != ' '){
        switch(stat){
            // moving and choosing of spell are allowed
            case STAY:{
                s.mapa->clear();
                switch(temp){
                    case 'f':{
                        common_menu->build_content(u);
                        common_menu->print();
                        print_help_for_spell_choose(manual);
                        stat = CHOOSE_SPELL;
                    }
                    break;

                    case 'i':{
                        s.common_log->hide();
                        bag->build_content(u);
                        bag->activate(0);
                        bag->print();
                        u->graph_state = GREEN_STABILE;
                        print_help_for_inventory(manual);
                        stat = OPEN_INVENTORY;
                    }
                    break;

                    case 'o':
                        u->graph_state = GREEN_STABILE;
                        search_targets(NULL, s, 0);
                        single_target = search_targets(u, s, INT_MAX);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;
                        oneline_log = new text_log(1, 50, 0, 63);
                        oneline_log->newline(single_target->get_name());
                        oneline_log->print();
                        print_help_for_observation_card(manual);
                        stat = OBSERVATION;
                        break;

                    default:
                        if(is_move_char(temp)){
                            single_target = (object*)s.mapa->get_object(u, temp);
                            if(single_target){
                                loot->build_content(single_target);
                                loot->activate(0);
                                loot->print();
                                u->graph_state = GREEN_STABILE;
                                print_help_for_loot(manual);
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
                        choosed_spell = common_menu->get_selected_value()->first;
                        clear_blinking((object**)s.mapa->objects);
                        u->graph_state = GREEN_STABILE;
                        s.mapa->clear();
                        switch (choosed_spell) {
                            case FIREBALL:
                                tar = new target(u->X, u->Y);
                                tar->graph_state = HIDE;
                                s.mapa->free_move(tar, 's');
                                s.mapa->draw_range(tar, 3);
                                print_help_for_spell_target_choose_ranged(manual);
                                break;

                            case PUNCH:
                                single_target = search_targets(u, s, 1);
                                last_color = single_target->graph_state;
                                single_target->graph_state = RED_INVERT;
                                print_help_for_spell_target_choose_single(manual);
                                s.mapa->update_card();
                                break;

                            case LIGHTING:
                                single_target = search_targets(u, s, 5);
                                last_color = single_target->graph_state;
                                single_target->graph_state = RED_INVERT;
                                print_help_for_spell_target_choose_single(manual);
                                s.mapa->update_card();
                                break;

                            default:
                                break;
                        }
                        common_menu->hide();
                        
                        stat = CHOOSE_TARGET;
                        break;

                    case 'q':
                        choosed_spell = NOTHING_SPELL;
                        print_help_for_stay(manual);
                        stat = STAY;
                        common_menu->delete_content();
                        common_menu->hide();
                        break;

                    case 'w':
                        common_menu->down();
                        break;

                    case 's':
                        common_menu->up();
                        break;

                    /*case 'h':
                        // spell help
                        // put hint about choosed spell
                        stat = SPELL_HELP;*/
                }
            }
            break;

            case CHOOSE_TARGET_FOR_ITEM:{
                if(is_item_for_single_target(item_to_use->type_name)){
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
                                    print_help_for_loot(manual);
                                    loot->print();
                                    break;
                                
                                case LOOT_INVENTORY:
                                    print_help_for_loot_inventory(manual);
                                    loot->print();

                                case OPEN_INVENTORY:
                                    print_help_for_inventory(manual);
                                    bag->print();

                                default:
                                    break;
                            }

                            break;

                        case 'f':{
                            single_target->graph_state = last_color;
                            for(spell_slot* s = item_to_use->slots; s; s = s->next)
                                if(s->config.trigger == BY_OWNER)
                                    ; // execute spells
                            
                            for(auto i = item_to_use->stat.effects.begin(); i != item_to_use->stat.effects.end(); i++){
                                if(is_shared(i->first) == SHARED){
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
                                goto link_0;
                                while(single_target){
                                    single_target->stat.act({1,FIRE_DAMAGE}, {0, 25});
                                    single_target->stat.act({1,MAGIC_ATTACK}, {0, 5});
                                    if(!single_target->is_alive()) single_target->graph_state = GRAY_STABILE;
                                link_0:
                                    single_target = search_targets(tar, s, 2);
                                }

                                s.common_log->print();

                                goto done;

                            case 'r':
                                print_help_for_spell_choose(manual);
                                stat = CHOOSE_SPELL;
                                s.mapa->clear();
                                u->graph_state = GREEN_ON;
                                common_menu->print();
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
                                
                                if(single_target->is_alive()) single_target->graph_state = last_color; 
                                else single_target->graph_state = GRAY_STABILE;

                            #if !defined(DONT_LOG_ACTIONS) || !defined(DONT_LOG_STATE)
                                s.common_log->print();
                            #endif

                                goto done;

                            case 'q':
                                print_help_for_spell_choose(manual);
                                stat = CHOOSE_SPELL;
                                s.mapa->clear();
                                u->graph_state = GREEN_ON;
                                common_menu->print();
                                s.mapa->update_card();
                                search_targets(nullptr, s, 0);
                                break;

                            default:
                                continue;
                        break;
                    }
                    break;


                    default:
                        break;
                }
                break;
            }

            case OPEN_INVENTORY:{
                switch(temp){
                    case 's':
                        bag->up();
                        break;

                    case 'w':
                        bag->down();
                        break;

                    case 'e':{
                        do_equp_unquip(bag, u);
                        bag->print();
                        break;
                    }

                    case 'u':{
                        if(bag->is_current_equiped()){
                            if(do_equp_unquip(bag, u)){
                                item_to_use = u->inventory.end() - 1;
                            }
                            else break;
                        }
                        else item_to_use = bag->get_selected_value().it;

                        looted_obj = u;

                        search_targets(NULL, s, 0);
                        single_target = search_targets(u, s, 4);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;

                        bag->hide();
                        s.mapa->clear();
                        s.mapa->update_card();

                        choosed_spell = NOTHING_SPELL;
                        last_state = OPEN_INVENTORY;
                        print_help_for_item_using(manual);
                        stat = CHOOSE_TARGET_FOR_ITEM;
                        break;
                    }

                    case 't':{
                        if(bag->is_current_equiped()){
                            if(do_equp_unquip(bag, u)){
                                item_to_use = u->inventory.end() - 1;
                            }
                            else break;
                        }
                        else item_to_use = bag->get_selected_value().it;

                        looted_obj = u;
                        u->item_to_use = &*item_to_use;

                        search_targets(NULL, s, 4);
                        single_target = search_targets(u, s, 4);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;

                        bag->hide();
                        s.mapa->clear();
                        s.mapa->update_card();

                        choosed_spell = THROW;
                        last_state = OPEN_INVENTORY;
                        print_help_for_spell_target_choose_single(manual);
                        stat = CHOOSE_TARGET;
                        break;
                    }

                    case 'q':
                        bag->hide();
                        bag->delete_content();
                        u->graph_state = GREEN_ON;
                        print_help_for_stay(manual);
                        stat = STAY;
                        break;
                }
            }
            break;

            /*case SPELL_HELP:
                switch(temp){
                    case 'q':
                        // clean hint
                        // need to return to right state.
                        // stat = CHOOSE_SPELL : SPELL_SHOP;
                        10;
                }
                break;*/

            case LOOT:{
                switch(temp){
                    case 'q':
                        loot->hide();
                        loot->delete_content();
                        u->graph_state = GREEN_ON;
                        print_help_for_stay(manual);
                        stat = STAY;
                        break;

                    case 's':
                        loot->up();
                        break;

                    case 'w':
                        loot->down();
                        break;

                    case 'e':{
                        do_equp_unquip(loot, single_target);
                        loot->print();
                        break;
                    }

                    case 'f':{
                        if(loot->is_current_equiped()){
                            if(do_equp_unquip(loot, single_target)){
                                u->pick_up_item(single_target->inventory.back());
                                single_target->inventory.pop_back();
                            }
                            else break;
                        }
                        else{
                            if(loot->size()){
                                auto num = loot->get_selected_value().it;
                                u->pick_up_item(*(num));
                                single_target->inventory.erase(num);

                                loot->delete_content();
                                loot->build_content(single_target);
                                loot->print();
                            }
                        }
                        break;
                    }

                    case 'u':{
                        if(loot->is_current_equiped()){
                            if(do_equp_unquip(loot, single_target)){
                                item_to_use = single_target->inventory.end() - 1;
                            }
                            else break;
                        }
                        else item_to_use = loot->get_selected_value().it;

                        looted_obj = single_target;

                        search_targets(NULL, s, 0);
                        single_target = search_targets(u, s, 4);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;

                        loot->hide();
                        s.mapa->clear();
                        s.mapa->update_card();

                        choosed_spell = NOTHING_SPELL;
                        last_state = LOOT;
                        print_help_for_item_using(manual);
                        stat = CHOOSE_TARGET_FOR_ITEM;
                        break;
                    }

                    case 't':{
                        if(loot->is_current_equiped()){
                            if(do_equp_unquip(loot, single_target)){
                                item_to_use = single_target->inventory.end() - 1;
                            }
                            else break;
                        }
                        else item_to_use = loot->get_selected_value().it;

                        looted_obj = single_target;
                        u->item_to_use = &*item_to_use;

                        search_targets(NULL, s, 0);
                        single_target = search_targets(u, s, 4);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;

                        loot->hide();
                        s.mapa->clear();
                        s.mapa->update_card();

                        choosed_spell = THROW;
                        last_state = LOOT;
                        print_help_for_spell_target_choose_single(manual);
                        stat = CHOOSE_TARGET;
                        break;
                    }

                    case 'i':{
                        bag->build_content(u);
                        bag->deactivate();
                        bag->print();
                        active_inv = loot;
                        print_help_for_loot_inventory(manual);
                        stat = LOOT_INVENTORY;
                        break;
                    }

                    case 'z':{
                        u->inventory.insert(u->inventory.cend(), single_target->inventory.begin(), single_target->inventory.end());
                        single_target->inventory.clear();
                        loot->delete_content();
                        loot->print();
                        break;
                    }
                }
            }
            break;

            case LOOT_INVENTORY:{
                switch(temp){
                    case 'i':
                        bag->hide();
                        bag->delete_content();
                        print_help_for_loot(manual);
                        stat = LOOT;
                        break;

                    case 'w':
                        active_inv->down();
                        break;

                    case 's':
                        active_inv->up();
                        break;

                    case 'a':
                        active_inv->deactivate();
                        bag->activate(active_inv->get_selected_index());
                        active_inv = bag;
                        bag->print();
                        loot->print();
                        break;

                    case 'd':
                        active_inv->deactivate();
                        loot->activate(active_inv->get_selected_index());
                        active_inv = loot;
                        bag->print();
                        loot->print();
                        break;

                    case 'u':
                        looted_obj = active_inv == bag ? u : single_target;

                        if(active_inv->is_current_equiped()){
                            if(do_equp_unquip(active_inv, looted_obj)){
                                item_to_use = looted_obj->inventory.end() - 1;
                            }
                            else break;
                        }
                        else item_to_use = active_inv->get_selected_value().it;

                        search_targets(NULL, s, 0);
                        single_target = search_targets(u, s, 4);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;

                        bag->hide();
                        loot->hide();
                        s.mapa->clear();
                        s.mapa->update_card();

                        choosed_spell = NOTHING_SPELL;
                        last_state = LOOT_INVENTORY;
                        print_help_for_item_using(manual);
                        stat = CHOOSE_TARGET_FOR_ITEM;
                        break;

                    case 't':
                        looted_obj = active_inv == bag ? u : single_target;

                        if(active_inv->is_current_equiped()){
                            if(do_equp_unquip(active_inv, looted_obj)){
                                item_to_use = looted_obj->inventory.end() - 1;
                            }
                            else break;
                        }
                        else item_to_use = active_inv->get_selected_value().it;

                        u->item_to_use = &*item_to_use;

                        search_targets(NULL, s, 0);
                        single_target = search_targets(u, s, 4);
                        last_color = single_target->graph_state;
                        single_target->graph_state = RED_INVERT;

                        bag->hide();
                        loot->hide();
                        s.mapa->clear();
                        s.mapa->update_card();

                        choosed_spell = THROW;
                        last_state = LOOT_INVENTORY;
                        print_help_for_spell_target_choose_single(manual);
                        stat = CHOOSE_TARGET;
                        break;

                    case 'e':{
                        if(active_inv->size() == 0) break;
                        object* tempor = active_inv == loot ? single_target : u;
                        do_equp_unquip(active_inv, tempor);
                        active_inv->print();
                        break;
                    }

                    case 'z':{
                        if(active_inv->size() == 0) break;

                        if(active_inv == loot){
                            u->inventory.insert(u->inventory.cend(), single_target->inventory.begin(), single_target->inventory.end());
                            single_target->inventory.clear();
                            bag->build_content(u);
                            loot->delete_content();
                        }
                        else{
                            single_target->inventory.insert(single_target->inventory.cend(), u->inventory.begin(), u->inventory.end());
                            u->inventory.clear();
                            loot->build_content(single_target);
                            bag->delete_content();
                        }

                        loot->print();
                        bag->print();
                        break;
                    }

                    case 'f':{
                        if(active_inv->size() == 0) break;
                                                
                        if(active_inv == loot){
                            if(active_inv->is_current_equiped()){
                                if(do_equp_unquip(active_inv, single_target)){
                                    u->pick_up_item(single_target->inventory.back());
                                    single_target->inventory.pop_back();
                                }
                                else break;
                            }
                            else{
                                auto thing = active_inv->get_selected_value().it;
                                u->pick_up_item(*thing);
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
                                auto thing = active_inv->get_selected_value().it;
                                single_target->pick_up_item(*(thing));
                                u->inventory.erase(thing);
                            }
                        }

                        bag->build_content(u);
                        bag->print();

                        loot->build_content(single_target);
                        loot->print();
                        break;
                    }

                    case 'q':
                        bag->hide();
                        bag->delete_content();
                        loot->hide();
                        loot->delete_content();
                        print_help_for_stay(manual);
                        stat = STAY;
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
                        oneline_log->newline(single_target->get_name());
                        oneline_log->print();
                        s.mapa->update_card();
                        break;

                    case 'f':
                        oneline_log->hide();
                        observe_menu->build_content(single_target);
                        observe_menu->print();
                        print_help_for_observation_objects(manual);
                        stat = CHOOSE_STATE_FOR_OBERVATION;
                        break;

                    case 'q':
                        oneline_log->hide();
                        delete oneline_log;
                        single_target->graph_state = GREEN_ON;
                        print_help_for_stay(manual);
                        stat = STAY;
                        break;
                }
            }
            break;

            case CHOOSE_STATE_FOR_OBERVATION:{
                switch (temp) {
                    case 'f':
                        observe_menu->hide();

                        observed_state =
                            observe_menu->get_selected_index() == 0 ?
                                &observe_menu->get_selected_value().owner->stat :
                                &observe_menu->get_selected_value().content.it->stat;

                        effect_screen = create_log_effects(observed_state);
                        effect_screen->print();
                        property_screen = create_log_properties(observed_state);
                        property_screen->print();
                        print_help_for_observation_state(manual);
                        stat = LOOKUP;
                        break;

                    case 'q':
                        observe_menu->hide();
                        oneline_log->print();
                        print_help_for_observation_card(manual);
                        stat = OBSERVATION;
                        break;

                    case 's':
                        observe_menu->up();
                        break;

                    case 'w':
                        observe_menu->down();
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

                        observe_menu->print();
                        print_help_for_observation_objects(manual);
                        stat = CHOOSE_STATE_FOR_OBERVATION;
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
    common_menu->hide();
    common_menu->delete_content();
    
    search_targets(nullptr, s, 0);

    return (temp == ' ' ? false : true);
}
