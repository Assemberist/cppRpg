#include "user_ifc_lib.hpp"

spell_menu::spell_menu(size_t rows, size_t cols, size_t pos_y, size_t pos_x)
    : menu<spell_menu_content*>(rows, cols, pos_y, pos_x)
    { strings = (char**)spell_names; }

void spell_menu::build_content(object* obj){
    delete_content();

    auto sp = obj->get_spells();
    content = new spell_menu_content*[sp.size()];
    count = 0;
    current = 0;

    for(auto i = sp.begin(); i != sp.end(); i++)
        content[count++] = &(*i);
}

void spell_menu::print(){
    wclear(win);
    for(int i = 0; i<count; i++){
        waddch(win, i == current ? '*' : ' ');
        wprintw(win, strings[content[i]->first]);
    }
    wrefresh(win);
}

//--------------------------------------------------------------------------------------------//
//   Inventory functions                                                                      //
//____________________________________________________________________________________________//

inventory::inventory(size_t rows, size_t cols, size_t pos_y, size_t pos_x)
    : menu<inventory_content>(rows, cols, pos_y, pos_x),
      isActive(false)
    { strings = (char**)item_names; }

void inventory::build_content(object* obj){
    current = 0;
    count = 0;
    content = new inventory_content[obj->inventory.size() + obj->equipment.size()];
    for(auto i = obj->equipment.begin(); i != obj->equipment.end(); i++){
        if(i->info.type_name == NOTHING_ITEM) continue;
        content[count].is_equiped = true;
        content[count].it = i;
        count++;
    }
    for(auto i = obj->inventory.begin(); i != obj->inventory.end(); i++){
        content[count].is_equiped = false;
        content[count].it = i;
        count++;
    }
}

void inventory::print(){
    wclear(win);

    if(count){
        for(size_t i = 0; i < count; i++){
            if(content[i].is_equiped)
                wattron(win, COLOR_PAIR(2));

            if(isActive)
                waddch(win, i == current ? '*' : ' ');
            wprintw(win, strings[content[i].it->info.type_name]);
        
            if(content[i].is_equiped)
                wattroff(win, COLOR_PAIR(2));
        }
    }
    else wprintw(win, "[inventory is empty]");

    wrefresh(win);
}

void inventory::activate(size_t num){
    isActive = true;
    current = num >= count ? count-1 : num;
}
void inventory::deactivate(){ isActive = false; }

bool inventory::is_current_equiped(){ return content[current].is_equiped; }
// void inventory::invert_equip(){ bag[current].is_equiped = !bag[current].is_equiped; }

inventory_with_owner::inventory_with_owner(size_t rows, size_t cols, size_t pos_y, size_t pos_x)
    : menu<inventory_content_2>(rows, cols, pos_y, pos_x),
      isActive(false)
    { strings = (char**)item_names; }

void inventory_with_owner::build_content(object* obj){
    current = 0;
    count = 1;
    content = new inventory_content_2[obj->inventory.size() + obj->equipment.size() + 1];
    content[0].owner = obj;

    for(auto i = obj->equipment.begin(); i != obj->equipment.end(); i++){
        if(i->info.type_name == NOTHING_ITEM) continue;
        content[count].content.is_equiped = true;
        content[count].content.it = i;
        count++;
    }
    for(auto i = obj->inventory.begin(); i != obj->inventory.end(); i++){
        content[count].content.is_equiped = false;
        content[count].content.it = i;
        count++;
    }
}

void inventory_with_owner::print(){
    wclear(win);

    size_t i = 0;
    waddch(win, i == current ? '*' : ' ');
    wprintw(win, content[0].owner->get_name());

    for(i = 1; i < count; i++){
        if(content[i].content.is_equiped)
            wattron(win, COLOR_PAIR(2));

        waddch(win, i == current ? '*' : ' ');
        wprintw(win, strings[content[i].content.it->info.type_name]);
    
        if(content[i].content.is_equiped)
            wattroff(win, COLOR_PAIR(2));
    }

    wrefresh(win);
}

bool inventory_with_owner::is_current_equiped(){ return content[current].content.is_equiped; }
