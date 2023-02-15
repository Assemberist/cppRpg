#include "user_ifc_lib.hpp"

log* create_log_effects(state* stat){
    char buffer[70];
    size_t amount = stat->effects.size();

    log* new_log = new log(10, 70, 1, 12);

    for(auto i = stat->effects.begin(); i != stat->effects.end(); i++){
        sprintf(buffer, "%30s Tim %4d Val %4d\n\0",
            effect_names[i->first.type],
            i->second.timed.time,
            i->second.timed.amount
        );

        new_log->newline(buffer);
    }

    return new_log;
}

log* create_log_properties(state* stat){
    char buffer[50];
    size_t amount = stat->propertyes.size();

    log* new_log = new log(10, 50, 12, 12);

    for(auto i = stat->propertyes.begin(); i != stat->propertyes.end(); i++){
        sprintf(buffer, "%30s Val %4d\n\0",
            property_names[i->first],
            i->second
        );

        new_log->newline(buffer);
    }

    return new_log;
}

void** create_bag(object* u, size_t& sas){
    sas = 0;
    void** bagaje = new void*[u->inventory.size() + u->equipment.size()];
    for(size_t i = u->equipment.size(); i--;){
        if(u->equipment[i].info.type_name == NOTHING_ITEM) continue;
        ((bag_element*)bagaje[sas])->is_equiped = true;
        ((bag_element*)bagaje[sas])->type = u->equipment[i].info.type_name;
        ((bag_element*)bagaje[sas])->element = i;
        sas++;
    }
    for(size_t i = u->inventory.size(); i--;){
        ((bag_element*)bagaje[sas])->is_equiped = false;
        ((bag_element*)bagaje[sas])->type = u->inventory[i].info.type_name;
        ((bag_element*)bagaje[sas])->element = i;
        sas++;
    }
    return bagaje;
}

void** create_bag_with_owner(object* u, size_t& sas){
    sas = 1;
    void** bagaje = new void*[u->inventory.size() + u->equipment.size() + 1];
    // First element is link to user.
    bagaje[0] = u;

    for(size_t i = u->equipment.size(); i--;){
        bagaje[sas] = new bag_element;
        if(u->equipment[i].info.type_name == NOTHING_ITEM) continue;
        ((bag_element*)bagaje[sas])->is_equiped = true;
        ((bag_element*)bagaje[sas])->type = u->equipment[i].info.type_name;
        ((bag_element*)bagaje[sas])->element = i;
        sas++;
    }
    for(size_t i = u->inventory.size(); i--;){
        bagaje[sas] = new bag_element;
        ((bag_element*)bagaje[sas])->is_equiped = false;
        ((bag_element*)bagaje[sas])->type = u->inventory[i].info.type_name;
        ((bag_element*)bagaje[sas])->element = i;
        sas++;
    }
    return bagaje;
}

void print_menu(WINDOW* win, size_t count, size_t current, void** elements, char** strings){
    for(int i = 0; i<count; i++){
        waddch(win, i == current ? '*' : ' ');
        wprintw(win, strings[((menu_element*)(elements[i]))->name]);
    }
}

void print_inventory_active(WINDOW* win, size_t count, size_t current, void** elements, char** strings){
    wclear(win);

    bag_element* current_item;

    if(count){
        for(size_t i = 0; i < count; i++){
            current_item = (bag_element*)(elements[i]);

            if(current_item->is_equiped)
                wattron(win, COLOR_PAIR(2));

            waddch(win, i == current ? '*' : ' ');
            wprintw(win, strings[current_item->type]);
        
            if(current_item->is_equiped)
                wattroff(win, COLOR_PAIR(2));
        }
    }
    else wprintw(win, "[inventory is empty]");

    wrefresh(win);

}

void print_inventory(WINDOW* win, size_t count, size_t current, void** elements, char** strings){
    wclear(win);

    bag_element* current_item;

    if(count){
        for(size_t i = 0; i < count; i++){
            current_item = (bag_element*)elements[i];

            if(current_item->is_equiped)
                wattron(win, COLOR_PAIR(2));

            wprintw(win, strings[current_item->type]);
        
            if(current_item->is_equiped)
                wattroff(win, COLOR_PAIR(2));
        }
    }
    else wprintw(win, "[inventory is empty]");

    wrefresh(win);
}
